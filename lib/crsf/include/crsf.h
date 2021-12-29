
#ifndef CRSF
    #define CRSF
    #define CRSF_UART_ID uart0
#define CRSF_BAUD_RATE 420000
#define CRSF_DATA_BITS 8
#define CRSF_DATA_LENGTH 64
#define CRSF_STOP_BITS 1
#define CRSF_UART_TX_PIN 0
#define CRSF_UART_RX_PIN 1
#define CRSF_PARITY UART_PARITY_NONE
#define CRSF_CHANNEL_VALUE_MIN 172
#define CRSF_CHANNEL_VALUE_1000 191
#define CRSF_CHANNEL_VALUE_MID 992
#define CRSF_CHANNEL_VALUE_2000 1792
#define CRSF_CHANNEL_VALUE_MAX 1811
#define CRSF_SYNC_BYTE 0XC8
/*
enum
{
    CRSF_FRAME_LENGTH_ADDRESS = 1,      // length of ADDRESS field
    CRSF_FRAME_LENGTH_FRAMELENGTH = 1,  // length of FRAMELENGTH field
    CRSF_FRAME_LENGTH_TYPE = 1,         // length of TYPE field
    CRSF_FRAME_LENGTH_CRC = 1,          // length of CRC field
    CRSF_FRAME_LENGTH_TYPE_CRC = 2,     // length of TYPE and CRC fields combined
    CRSF_FRAME_LENGTH_EXT_TYPE_CRC = 4, // length of Extended Dest/Origin, TYPE and CRC fields combined
    CRSF_FRAME_LENGTH_NON_PAYLOAD = 4,  // combined length of all fields except payload
};

enum
{
    CRSF_FRAME_GPS_PAYLOAD_SIZE = 15,
    CRSF_FRAME_BATTERY_SENSOR_PAYLOAD_SIZE = 8,
    CRSF_FRAME_LINK_STATISTICS_PAYLOAD_SIZE = 10,
    CRSF_FRAME_RC_CHANNELS_PAYLOAD_SIZE = 22, // 11 bits per channel * 16 channels = 22 bytes.
    CRSF_FRAME_ATTITUDE_PAYLOAD_SIZE = 6,
};
bool CRSF_sync;

typedef struct crsf_header_s
{
    uint8_t device_addr; // from crsf_addr_e
    uint8_t frame_size;  // counts size after this byte, so it must be the payload size + 2 (type and crc)
    uint8_t type;        // from crsf_frame_type_e
    uint8_t data[0];
    };

    int CRSF_channels[16];
    bool CRSF_failsafe;
    bool CRSF_lost_frame;
    extern int CRSF_init();
    extern int CRSF_getChannel();
    typedef struct crsfPayloadLinkstatistics_s
    {
        uint8_t uplink_RSSI_1;
        uint8_t uplink_RSSI_2;
        uint8_t uplink_Link_quality;
        int8_t uplink_SNR;
        uint8_t active_antenna;
        uint8_t rf_Mode;
        uint8_t uplink_TX_Power;
        uint8_t downlink_RSSI;
        uint8_t downlink_Link_quality;
        int8_t downlink_SNR;
    } crsfLinkStatistics_t;

    typedef struct crsf_sensor_battery_s
    {
        unsigned voltage : 16;  // V * 10 big endian
        unsigned current : 16;  // A * 10 big endian
        unsigned capacity : 24; // mah
        unsigned remaining : 8; // %
    };*/
#endif