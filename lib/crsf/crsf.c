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

absolute_time_t last_byte_time; // timestamp of last byte we received
absolute_time_t current_byte_time; // timestamp of current byte we received, we then compare it with last_byte_time
absolute_time_t framestart; // timestamp of start of frame it will be compared to the current time (not variable), to check for timeout
bool first_byte_rx = false; // when we run the CRSF_on_uart_rx there will not be any information about last_byte_time, so we need to wait for one cycle
int64_t time_diff = 0; // time difference between last_byte_time and current_byte_time
crsfFrameDef_t Frame; // current crsf Frame

void CRSF_on_uart_rx() // IRQ handler, called when there are data on UART port
{
    while (uart_is_readable(CRSF_UART_ID)) // while we can read UART data we read it from
    {

        uint8_t ch = uart_getc(CRSF_UART_ID); // this will read character from uart and save it to ch. This function is mandatory in this loop, without this the loop will become infinite

        current_byte_time = get_absolute_time(); // get current byte time
        time_diff = absolute_time_diff_us(last_byte_time, current_byte_time); // compare the current byte time with the last byte time
        last_byte_time = current_byte_time; // after we get the difference of two times, we save the current byte time to the last byte time

        //printf("byte: %x, latency: %d\n", ch, time_diff); //debug function to printout the byte and latency
        if (time_diff > CRSF_TIME_BETWEEN_FRAMES_US && ch == CRSF_SYNC_BYTE) // if the time between last byte and current byte time is greater than minimum time between frames we will call that start of packet, and if it the character matches with flightcontroller address we assume this is for us
        {
            if(CRSF_sync){ //if new packet started but we were the previous packet wasn't read completely we will call that desync 
                printf("desync 3, %d\n", CRSF_chars_rxed);
            }
            CRSF_chars_rxed = 0; // set current byte index to 0

            CRSF_sync = true; // set sync flag to true
            Frame.deviceAddress = ch; // set device address (probably completly uneccesary)
        }

        if (CRSF_sync) // if we are synced we will run this code
        {
            if(CRSF_chars_rxed == 0){ // if this is first byte we will save time
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
           
            if (CRSF_chars_rxed < CRSF_DATA_LENGTH)
            { // if the CRSF_chars_rxed isn't larger than maximum length value we will save byte
                CRSF_bufferRX[CRSF_chars_rxed] = ch;
            }

            else if (CRSF_chars_rxed > CRSF_DATA_LENGTH)
            { // if it is we will desync
                CRSF_chars_rxed = 0;
                CRSF_sync = false;
            }

            CRSF_chars_rxed++;
            
        }
        if (CRSF_chars_rxed == (Frame.frameLength - 1) && CRSF_sync)
        { // if we are synced and the byte index matches the framelength, We have a full packet. nice
            printf("\nFrame length: %d\n", Frame.frameLength); //debug
            printf("bytes in packet: %d\n", CRSF_chars_rxed + 1); // debug
            CRSF_chars_rxed = 0; //reset so we can start again
            CRSF_sync = false;   // reset so we can start again
            
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
    //gpio_set_inover(CRSF_UART_RX_PIN, GPIO_OVERRIDE_INVERT);

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
