
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
#define CRSF_SYNC_BYTE 0XC8 // this is the ADDRESS, this byte is the first byte of the packet
#define CRSF_TIME_NEEDED_PER_FRAME_US 1100 // us ms + 400 ms for potential ad-hoc request
#define CRSF_TIME_BETWEEN_FRAMES_US 6667   // At fastest, frames are sent by the transmitter every 6.667 milliseconds, 150 Hz

#define CRSF_DIGITAL_CHANNEL_MIN 172
#define CRSF_DIGITAL_CHANNEL_MAX 1811

#define CRSF_PAYLOAD_OFFSET offsetof(crsfFrameDef_t, type)

#define CRSF_LINK_STATUS_UPDATE_TIMEOUT_US 250000 // 250ms, 4 Hz mode 1 telemetry

#define CRSF_FRAME_ERROR_COUNT_THRESHOLD 100

#define CRSF_MAX_CHANNEL 16
#define CRSFV3_MAX_CHANNEL 24

#define CRSF_SUBSET_RC_STARTING_CHANNEL_BITS 5
#define CRSF_SUBSET_RC_STARTING_CHANNEL_MASK 0x1F
#define CRSF_SUBSET_RC_RES_CONFIGURATION_BITS 2
#define CRSF_SUBSET_RC_RES_CONFIGURATION_MASK 0x03
#define CRSF_SUBSET_RC_RESERVED_CONFIGURATION_BITS 1

#define CRSF_RC_CHANNEL_SCALE_LEGACY 0.62477120195241f
#define CRSF_SUBSET_RC_RES_CONF_10B 0
#define CRSF_SUBSET_RC_RES_BITS_10B 10
#define CRSF_SUBSET_RC_RES_MASK_10B 0x03FF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_10B 1.0f
#define CRSF_SUBSET_RC_RES_CONF_11B 1
#define CRSF_SUBSET_RC_RES_BITS_11B 11
#define CRSF_SUBSET_RC_RES_MASK_11B 0x07FF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_11B 0.5f
#define CRSF_SUBSET_RC_RES_CONF_12B 2
#define CRSF_SUBSET_RC_RES_BITS_12B 12
#define CRSF_SUBSET_RC_RES_MASK_12B 0x0FFF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_12B 0.25f
#define CRSF_SUBSET_RC_RES_CONF_13B 3
#define CRSF_SUBSET_RC_RES_BITS_13B 13
#define CRSF_SUBSET_RC_RES_MASK_13B 0x1FFF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_13B 0.125f
#define CRSF_PAYLOAD_SIZE_MAX 64
#define CRSF_RSSI_MIN (-130)
#define CRSF_RSSI_MAX 0
#define CRSF_SNR_MIN (-30)
#define CRSF_SNR_MAX 20
#define CRSF_FAILSAFE_STAGE1_US 300000


enum CRSFDataOffset
{
    CRSF_FRAME_LENGTH_ADDRESS = 1,      // length of ADDRESS field
    CRSF_FRAME_LENGTH_FRAMELENGTH = 1,  // length of FRAMELENGTH field
    CRSF_FRAME_LENGTH_TYPE = 1,         // length of TYPE field
    CRSF_FRAME_LENGTH_CRC = 1,          // length of CRC field
    CRSF_FRAME_LENGTH_TYPE_CRC = 2,     // length of TYPE and CRC fields combined
    CRSF_FRAME_LENGTH_EXT_TYPE_CRC = 4, // length of Extended Dest/Origin, TYPE and CRC fields combined
    CRSF_FRAME_LENGTH_NON_PAYLOAD = 4,  // combined length of all fields except payload
};

enum TelemetryDataOffset
{
    CRSF_FRAME_GPS_PAYLOAD_SIZE = 15,
    CRSF_FRAME_BATTERY_SENSOR_PAYLOAD_SIZE = 8,
    CRSF_FRAME_LINK_STATISTICS_PAYLOAD_SIZE = 10,
    CRSF_FRAME_RC_CHANNELS_PAYLOAD_SIZE = 22, // 11 bits per channel * 16 channels = 22 bytes.
    CRSF_FRAME_ATTITUDE_PAYLOAD_SIZE = 6,
};
typedef enum
{
    CRSF_ADDRESS_BROADCAST = 0x00,
    CRSF_ADDRESS_USB = 0x10,
    CRSF_ADDRESS_TBS_CORE_PNP_PRO = 0x80,
    CRSF_ADDRESS_RESERVED1 = 0x8A,
    CRSF_ADDRESS_CURRENT_SENSOR = 0xC0,
    CRSF_ADDRESS_GPS = 0xC2,
    CRSF_ADDRESS_TBS_BLACKBOX = 0xC4,
    CRSF_ADDRESS_FLIGHT_CONTROLLER = 0xC8,
    CRSF_ADDRESS_RESERVED2 = 0xCA,
    CRSF_ADDRESS_RACE_TAG = 0xCC,
    CRSF_ADDRESS_RADIO_TRANSMITTER = 0xEA,
    CRSF_ADDRESS_CRSF_RECEIVER = 0xEC,
    CRSF_ADDRESS_CRSF_TRANSMITTER = 0xEE,
} crsf_addr_e;

typedef enum
{
    CRSF_FRAMETYPE_GPS = 0x02,
    CRSF_FRAMETYPE_BATTERY_SENSOR = 0x08,
    CRSF_FRAMETYPE_LINK_STATISTICS = 0x14,
    CRSF_FRAMETYPE_RC_CHANNELS_PACKED = 0x16,
    CRSF_FRAMETYPE_SUBSET_RC_CHANNELS_PACKED = 0x17,
    CRSF_FRAMETYPE_LINK_STATISTICS_RX = 0x1C,
    CRSF_FRAMETYPE_LINK_STATISTICS_TX = 0x1D,
    CRSF_FRAMETYPE_ATTITUDE = 0x1E,
    CRSF_FRAMETYPE_FLIGHT_MODE = 0x21,
    // Extended Header Frames, range: 0x28 to 0x96
    CRSF_FRAMETYPE_DEVICE_PING = 0x28,
    CRSF_FRAMETYPE_DEVICE_INFO = 0x29,
    CRSF_FRAMETYPE_PARAMETER_SETTINGS_ENTRY = 0x2B,
    CRSF_FRAMETYPE_PARAMETER_READ = 0x2C,
    CRSF_FRAMETYPE_PARAMETER_WRITE = 0x2D,
    CRSF_FRAMETYPE_COMMAND = 0x32,
    // MSP commands
    CRSF_FRAMETYPE_MSP_REQ = 0x7A,         // response request using msp sequence as command
    CRSF_FRAMETYPE_MSP_RESP = 0x7B,        // reply with 58 byte chunked binary
    CRSF_FRAMETYPE_MSP_WRITE = 0x7C,       // write with 8 byte chunked binary (OpenTX outbound telemetry buffer limit)
    CRSF_FRAMETYPE_DISPLAYPORT_CMD = 0x7D, // displayport control command
} crsfFrameType_e;

typedef struct crsfFrameDef_s
{
    uint8_t deviceAddress;
    uint8_t frameLength;
    uint8_t type;
    uint8_t payload[CRSF_PAYLOAD_SIZE_MAX + 1]; // +1 for CRC at end of payload
} crsfFrameDef_t;

typedef struct crsfLink_s
{
    uint8_t RSSI_uplink_a1;
    uint8_t RSSI_uplink_a2;
    uint8_t LQ_uplink;
    uint8_t SNR_uplink;
    uint8_t DIVERSITY_ant;
    uint8_t RF_mode;
    uint8_t TXPOW_uplink;
    uint8_t RSSI_downlink;
    uint8_t LQ_downlink;
    uint8_t SNR_downlink;
} crsfLink_t;

extern int CRSF_channels[16];
bool CRSF_failsafe();
crsfLink_t CRSF_getLinkStatus();
extern int CRSF_init();
extern int CRSF_getChannel();
extern const int CRSF_channelsLength;
bool CRSF_sync;
#endif