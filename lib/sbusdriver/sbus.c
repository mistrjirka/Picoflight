#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "sbus.h"


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

static char chars_rxed = 0;
static int chars_packet_length = 0;
uint8_t bufferValidity[1] = {0};
bool sync = false;
bool lost_frame = false, failsafe = false;

uint8_t bufferRX[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int channels[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void on_uart_rx()
{
    // printf("uart rx %d \n", chars_packet_length);
    while (uart_is_readable(UART_ID))
    {

        uint8_t ch = uart_getc(UART_ID);
        if (sync)
        {
            bufferRX[chars_packet_length] = ch;
            chars_packet_length++;
        }
        chars_rxed++;
        if(ch == 15){
            sync = true;
        }
        
    }
    if (chars_packet_length == 24)
    {
        //printf("completed one packet test:\n");
        int channel = 0;
        bool ignore = false;

        if (sync)
        {
            channels[0] = (uint16_t)((bufferRX[0] | bufferRX[1] << 8) & 0x07FF);
            channels[1] = (uint16_t)((bufferRX[1] >> 3 | bufferRX[2] << 5) & 0x07FF);
            channels[2] = (uint16_t)((bufferRX[2] >> 6 | bufferRX[3] << 2 | bufferRX[4] << 10) & 0x07FF);
            channels[3] = (uint16_t)((bufferRX[4] >> 1 | bufferRX[5] << 7) & 0x07FF);
            channels[4] = (uint16_t)((bufferRX[5] >> 4 | bufferRX[6] << 4) & 0x07FF);
            channels[5] = (uint16_t)((bufferRX[6] >> 7 | bufferRX[7] << 1 | bufferRX[8] << 9) & 0x07FF);
            channels[6] = (uint16_t)((bufferRX[8] >> 2 | bufferRX[9] << 6) & 0x07FF);
            channels[7] = (uint16_t)((bufferRX[9] >> 5 | bufferRX[10] << 3) & 0x07FF);
            channels[8] = (uint16_t)((bufferRX[11] | bufferRX[12] << 8) & 0x07FF);
            channels[9] = (uint16_t)((bufferRX[12] >> 3 | bufferRX[13] << 5) & 0x07FF);
            channels[10] = (uint16_t)((bufferRX[13] >> 6 | bufferRX[14] << 2 | bufferRX[15] << 10) & 0x07FF);
            channels[11] = (uint16_t)((bufferRX[15] >> 1 | bufferRX[16] << 7) & 0x07FF);
            channels[12] = (uint16_t)((bufferRX[16] >> 4 | bufferRX[17] << 4) & 0x07FF);
            channels[13] = (uint16_t)((bufferRX[17] >> 7 | bufferRX[18] << 1 | bufferRX[19] << 9) & 0x07FF);
            channels[14] = (uint16_t)((bufferRX[19] >> 2 | bufferRX[20] << 6) & 0x07FF);
            channels[15] = (uint16_t)((bufferRX[20] >> 5 | bufferRX[21] << 3) & 0x07FF);
            channels[16] = bufferRX[22] & CH17_MASK_;
            /* CH 18 */
            channels[17] = bufferRX[22] & CH18_MASK_;
            /* Grab the lost frame */
            lost_frame = bufferRX[22] & LOST_FRAME_MASK_;
            /* Grab the failsafe */
            failsafe = bufferRX[22] & FAILSAFE_MASK_;
            //printf("%x", bufferRX[22]);
            //printf("%x", FAILSAFE_MASK_);
            sync = false;
        }
        chars_packet_length = 0;
        

            /*for (int i = 0; i < 18; ++i)
            {
                printf("\n  channels: %d", channels[i]);
                // printf("\n channel %d: %d ", i, channels[i]);
            }
            printf("\n failsafe %d", failsafe);
            printf("\n lost frames %d", lost_frame);*/

            
            //printf("\nEnd\n");
        }
}

int SBUS_getChannel(int channel)
{
    if (channel < 19){
        return channels[channel-1];
    }else{
        return 0;
    }
}

int SBUS_init()
{
    int baud = uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_inover(UART_RX_PIN, GPIO_OVERRIDE_INVERT);

    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

   
}
