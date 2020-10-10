#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG_MODE false
#if DEBUG_MODE
#define debugSerial Serial
#define DEBUG_PRINT(x) debugSerial.print(x)
#define DEBUG_PRINTLN(x) debugSerial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
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

#define DEFAULT_SERVO_UP_VALUE 135
#define DEFAULT_SERVO_DOWN_VALUE 90

typedef enum
{
    MOTOR_1,
    MOTOR_2,
    MOTOR_MAX,
} MOTOR_E;

typedef enum
{
    IDLE,
    INIT,
    READY,
} STATE_E; 

#endif