#ifndef HC06_H
#define HC06_H

#include <Arduino.h>

typedef enum
{
    ERROR_HC06_NONE,
    ERROR_HC06_RXTX_PIN,
    ERROR_HC06_BAUDRATE,
    ERROR_HC06_NO_DATA,
} ERROR_CODE_E;

typedef enum
{
    RXCMD_A,
    RXCMD_B,
    RXCMD_C,
    RXCMD_D,
    RXCMD_E,
    RXCMD_F,
    RXCMD_MAX
} SIM7020_RXCMD_E;

typedef enum
{
    RX_HEADER = 0,
    RX_CONTENT,
    RX_DATA,
    RX_FOOTER,
    RX_ERROR,
    RX_MAX
} SIM7020_RX_STATE_E;

typedef enum
{
    MSG_NONE = 0,
    MSG_COLON,
    MSG_MAX
} RX_MSG_TYPE_E;

typedef struct {
    const char *  command;     // 收到的 COMMAND
    uint8_t       length;      // COMMAND 長度，不含特殊字元
    RX_MSG_TYPE_E type;        // 類型
    char *        data;        // 指向 data 的指標
} rx_message_t;

int readHC06Msg(void);
int initHC06(uint8_t rx_pin, uint8_t tx_pin, uint32_t baudrate);

#endif