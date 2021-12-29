#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "crsf.h"


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

static char CRSF_chars_rxed = 0;
static int CRSF_chars_packet_length = 0;
uint8_t CRSF_bufferValidity[1] = {0};
bool CRSF_sync = false;
bool CRSF_lost_frame = false, CRSF_failsafe = false;

uint8_t bufferRX[64];
int CRSF_channels[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void CRSF_on_uart_rx()
{

    //printf("test %d\n", uart_is_readable(CRSF_UART_ID));

    // CRSF_sync = !uart_is_readable(CRSF_UART_ID);
    while (uart_is_readable(CRSF_UART_ID))
    {

        uint8_t ch = uart_getc(CRSF_UART_ID);
        printf("%x \n", ch);
        
    }
}

int CRSF_getChannel(int channel)
{
    if (channel < 17){
        return CRSF_channels[channel-1];
    }else{
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
