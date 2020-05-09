#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG_MODE true
#if DEBUG_MODE
#define debugSerial Serial
#define DEBUG_PRINT(x) debugSerial.println(x)
#else
#define DEBUG_PRINT(x)
#endif

#define BAUDRATE_9600 9600
#define BAUDRATE_38400 38400
#define BAUDRATE_115200 115200

#define BT_RX 2
#define BT_TX 3

#define SET_BIT(x, n) ((x) |= (1 << (n)))
#define CLR_BIT(x, n) ((x) &= (~(1 << (n))))
#define CHK_BIT(x, n) (((x) & (1 << (n))) != 0)
#define TGL_BIT(x, n) ((x) ^= (1 << (n)))

typedef enum
{
    IDLE,
    INIT,
    READY,
} STATE_E;

#endif