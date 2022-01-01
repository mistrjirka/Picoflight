#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/multicore.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "crsf.h"
#include "pico/multicore.h"

/*
#define MOTOR0_OUT = 10

#define SERVO0_OUT = 7
#define SERVO0_MIN = 0
#define SERVO0_MAX = 180

#define SERVO1_OUT = 6
#define SERVO0_MIN = 0
#define SERVO0_MAX = 180

#define SIGNAL_MIN = 172
#define SIGNAL_MAX = 1810
*/

int CRSF_chars_rxed = 0;
uint8_t CRSF_bufferValidity[1] = {0};
bool CRSF_sync = false;
bool CRSF_lost_frame = false, CRSF_failsafe = false;
bool CRSF_preSync = false;
uint8_t CRSF_bufferRX[64];
int CRSF_channels[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int columnLength = 5;
int columnIndex = 0;

absolute_time_t last_byte_time;    // timestamp of last byte we received
absolute_time_t current_byte_time; // timestamp of current byte we received, we then compare it with last_byte_time
absolute_time_t framestart;        // timestamp of start of frame it will be compared to the current time (not variable), to check for timeout
bool first_byte_rx = false;        // when we run the CRSF_on_uart_rx there will not be any information about last_byte_time, so we need to wait for one cycle
int64_t time_diff = 0;             // time difference between last_byte_time and current_byte_time
crsfFrameDef_t Frame;              // current crsf Frame
crsfLink_t LinkStatus;

struct crsfPayloadRcChannelsPacked_s // from betaflight
{
    // 176 bits of data (11 bits per channel * 16 channels) = 22 bytes.
    // the ": 11" means the number is 11 bit. How? or what it means? Idk
    unsigned int chan0 : 11;
    unsigned int chan1 : 11;
    unsigned int chan2 : 11;
    unsigned int chan3 : 11;
    unsigned int chan4 : 11;
    unsigned int chan5 : 11;
    unsigned int chan6 : 11;
    unsigned int chan7 : 11;
    unsigned int chan8 : 11;
    unsigned int chan9 : 11;
    unsigned int chan10 : 11;
    unsigned int chan11 : 11;
    unsigned int chan12 : 11;
    unsigned int chan13 : 11;
    unsigned int chan14 : 11;
    unsigned int chan15 : 11;
} __attribute__((__packed__));
typedef struct crsfPayloadRcChannelsPacked_s crsfPayloadRcChannelsPacked_t; // from betaflight

void CRSF_on_uart_rx() // IRQ handler, called when there are data on UART port
{
    while (uart_is_readable(CRSF_UART_ID)) // while we can read UART data we read it from
    {

        uint8_t ch = uart_getc(CRSF_UART_ID); // this will read character from uart and save it to ch. This function is mandatory in this loop, without this the loop will become infinite

        current_byte_time = get_absolute_time();                              // get current byte time
        time_diff = absolute_time_diff_us(last_byte_time, current_byte_time); // compare the current byte time with the last byte time
        last_byte_time = current_byte_time;                                   // after we get the difference of two times, we save the current byte time to the last byte time

        // printf("byte: %x, latency: %d\n", ch, time_diff); //debug function to printout the byte and latency
        if (time_diff > CRSF_TIME_BETWEEN_FRAMES_US && ch == CRSF_SYNC_BYTE) // if the time between last byte and current byte time is greater than minimum time between frames we will call that start of packet, and if it the character matches with flightcontroller address we assume this is for us
        {
            if (CRSF_sync)
            { // if new packet started but we were the previous packet wasn't read completely we will call that desync
                printf("desync 3, %d\n", CRSF_chars_rxed);
            }
            CRSF_chars_rxed = 0; // set current byte index to 0

            CRSF_sync = true;         // set sync flag to true
            Frame.deviceAddress = ch; // set device address (probably completly uneccesary)
        }

        if (CRSF_sync) // if we are synced we will run this code
        {
            if (CRSF_chars_rxed == 0)
            { // if this is first byte we will save time
                framestart = current_byte_time;
            }
            if (CRSF_chars_rxed < 1) // if we hadn't get the length of frame yet, we will set it to some arbitrary value
            {
                Frame.frameLength = 5;
            }

            if (CRSF_chars_rxed == 1) // This is the packet length byte
            {

                if (ch > CRSF_DATA_LENGTH || ch < 4) // if packet length is longer than maximum packet length or it is too short we will set it to desync and set byte index to 0 to start from scratch
                {
                    CRSF_sync = false;
                    CRSF_chars_rxed = 0;
                }
                else
                { // packet as long as it should be

                    Frame.frameLength = ch;
                }
            }

            if (CRSF_chars_rxed == 2)
            { // if this is the third byte it is the packet type byte

                Frame.type = ch;
            }

            if (CRSF_chars_rxed < CRSF_DATA_LENGTH && CRSF_chars_rxed > 2)
            { // if the CRSF_chars_rxed isn't larger than maximum length value we will save byte
                Frame.payload[CRSF_chars_rxed-3] = ch;
            }

            else if (CRSF_chars_rxed > CRSF_DATA_LENGTH)
            { // if it is we will desync
                CRSF_chars_rxed = 0;
                CRSF_sync = false;
            }

            CRSF_chars_rxed++;
        }
        if (CRSF_chars_rxed == (Frame.frameLength + 2) && CRSF_sync)
        {                                                         // if we are synced and the byte index matches the framelength, We have a full packet. nice
            //printf("\nFrame length: %d\n", Frame.frameLength);    // debug
            //printf("bytes in packet: %d\n", CRSF_chars_rxed + 1); // debug
            CRSF_chars_rxed = 0;                                  // reset so we can start again
            CRSF_sync = false;                                    // reset so we can start again
            switch (Frame.type)
            {
            case CRSF_FRAMETYPE_RC_CHANNELS_PACKED:
                //if this is rc frame
                printf("RC frame received\n");
                const crsfPayloadRcChannelsPacked_t *const rcChannels = (crsfPayloadRcChannelsPacked_t *)&Frame.payload;
                uint8_t channelScale = CRSF_RC_CHANNEL_SCALE_LEGACY;
                CRSF_channels[0] = rcChannels->chan0;
                CRSF_channels[1] = rcChannels->chan1;
                CRSF_channels[2] = rcChannels->chan2;
                CRSF_channels[3] = rcChannels->chan3;
                CRSF_channels[4] = rcChannels->chan4;
                CRSF_channels[5] = rcChannels->chan5;
                CRSF_channels[6] = rcChannels->chan6;
                CRSF_channels[7] = rcChannels->chan7;
                CRSF_channels[8] = rcChannels->chan8;
                CRSF_channels[9] = rcChannels->chan9;
                CRSF_channels[10] = rcChannels->chan10;
                CRSF_channels[11] = rcChannels->chan11;
                CRSF_channels[12] = rcChannels->chan12;
                CRSF_channels[13] = rcChannels->chan13;
                CRSF_channels[14] = rcChannels->chan14;
                CRSF_channels[15] = rcChannels->chan15;
                printf("Channel 1: %d\n", CRSF_channels[0]);
                printf("Channel 2: %d\n", CRSF_channels[1]);
                printf("Channel 3: %d\n", CRSF_channels[2]);
                printf("Channel 4: %d\n", CRSF_channels[3]);
                printf("Channel 5: %d\n", CRSF_channels[4]);
                printf("Channel 6: %d\n", CRSF_channels[5]);
                printf("Channel 7: %d\n", CRSF_channels[6]);
                printf("Channel 8: %d\n", CRSF_channels[7]);
                printf("Channel 9: %d\n", CRSF_channels[8]);
                printf("Channel 10: %d\n", CRSF_channels[9]);
                printf("Channel 11: %d\n", CRSF_channels[10]);
                break;
            case CRSF_FRAMETYPE_SUBSET_RC_CHANNELS_PACKED:
                break;
            case CRSF_FRAMETYPE_LINK_STATISTICS:

                // if to FC and 10 bytes + CRSF_FRAME_ORIGIN_DEST_SIZE

                //printf("statistics frame received\n");
                LinkStatus.RSSI_uplink_a1 = Frame.payload[0]; // antenna 1 uplink RSSI;
                LinkStatus.RSSI_uplink_a2 = Frame.payload[1];  // antenna 2 uplink RSSI;
                LinkStatus.LQ_uplink = Frame.payload[2];       // antenna 1 uplink RSSI;
                LinkStatus.SNR_uplink = Frame.payload[3];      // antenna 1 uplink RSSI;
                LinkStatus.DIVERSITY_ant = Frame.payload[4];
                LinkStatus.RF_mode = Frame.payload[5];
                LinkStatus.TXPOW_uplink = Frame.payload[6];
                LinkStatus.RSSI_downlink = Frame.payload[7];
                LinkStatus.LQ_downlink = Frame.payload[8];
                LinkStatus.SNR_downlink = Frame.payload[9];


                /*printf("RSSI a1: %d\n", LinkStatus.RSSI_uplink_a1);
                printf("RSSI a2: %d\n", LinkStatus.RSSI_uplink_a2);
                printf("LQ up: %d\n", LinkStatus.LQ_uplink);
                printf("SNR up: %d\n", LinkStatus.SNR_uplink);
                printf("DIVERSITY a: %d\n", LinkStatus.DIVERSITY_ant);
                printf("RF mode: %d\n", LinkStatus.RF_mode);
                printf("TXPOW up: %d\n", LinkStatus.TXPOW_uplink);
                printf("RSSI downlink: %d\n", LinkStatus.RSSI_downlink);
                printf("LQ Downlink: %d\n", LinkStatus.LQ_downlink);
                printf("SNR Downlink: %d\n", LinkStatus.SNR_downlink);
                printf("CRC:  %d\n", Frame.payload[10]);*/

                break;
            default:
                break;
            }
        }
    }
}

int CRSF_getChannel(int channel)
{
    if (channel < 17)
    {
        return CRSF_channels[channel - 1];
    }
    else
    {
        return 0;
    }
}

int CRSF_init()
{
    int baud = uart_init(CRSF_UART_ID, CRSF_BAUD_RATE);

    gpio_set_function(CRSF_UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(CRSF_UART_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_inover(CRSF_UART_RX_PIN, GPIO_OVERRIDE_INVERT);

    uart_set_hw_flow(CRSF_UART_ID, false, false);

    // Set our data format
    uart_set_format(CRSF_UART_ID, CRSF_DATA_BITS, CRSF_STOP_BITS, CRSF_PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(CRSF_UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = CRSF_UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, CRSF_on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    printf("hello there\n");
    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(CRSF_UART_ID, true, false);
    printf("enabled\n");
}
