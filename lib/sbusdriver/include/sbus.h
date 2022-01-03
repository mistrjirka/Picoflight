
#ifndef SBUS
    #define SBUS
#define SBUS_UART_ID uart0
#define SBUS_BAUD_RATE 100000
#define SBUS_DATA_BITS 8
#define SBUS_STOP_BITS 2
#define SBUS_UART_TX_PIN 0
#define SBUS_UART_RX_PIN 1
#define SBUS_PARITY UART_PARITY_EVEN

static uint8_t SBUS_CH17_MASK_ = 0x01;
static uint8_t SBUS_CH18_MASK_ = 0x02;
static uint8_t SBUS_LOST_FRAME_MASK_ = 0x04;
static uint8_t SBUS_FAILSAFE_MASK_ = 0x08;
int channels[18];
bool SBUS_failsafe;
bool SBUS_lost_frame;
#define SBUS_NUM_CHANNELS = 18;
extern int SBUS_init();
extern int SBUS_getChannel();
#endif