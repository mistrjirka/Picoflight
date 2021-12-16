
#ifndef SBUS
    #define SBUS
    #define UART_ID uart0
    #define BAUD_RATE 100000
    #define DATA_BITS 8
    #define STOP_BITS 2
    #define UART_TX_PIN 0
    #define UART_RX_PIN 1
    #define PARITY UART_PARITY_EVEN

    static uint8_t CH17_MASK_ = 0x01;
    static uint8_t CH18_MASK_ = 0x02;
    static uint8_t LOST_FRAME_MASK_ = 0x04;
    static uint8_t FAILSAFE_MASK_ = 0x08;
    int channels[18];
    bool failsafe;
    bool lost_frame;
    #define NUM_CHANNELS = 18;
    extern int SBUS_init();
    extern int SBUS_getChannel();
#endif