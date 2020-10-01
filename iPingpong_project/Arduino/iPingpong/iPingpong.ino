#include "HC06.h"
#include "config.h"
#include <Arduino.h>

uint32_t msg_flags;

// FIXME: RENAME
char A[4];
char B[4];
char C[4];
char D[4];
char R[4];
char S[4];
char E[4];

const rx_message_t message[8] =  {{"A", 1, MSG_COLON, A},   {"B", 1, MSG_COLON, B},
                                 {"C", 1, MSG_COLON, C},   {"D", 1, MSG_COLON, D},
                                 {"R", 1, MSG_COLON, R},   {"S", 1, MSG_COLON, S},
                                 {"E", 1, MSG_COLON, E},   {NULL, 0, MSG_NONE, NULL}};

uint8_t iping_state = INIT;

void task(void);

void setup()
{
#if DEBUG_MODE
    debugSerial.begin(BAUDRATE_9600);
#endif
    initHC06(BT_RX, BT_TX, BAUDRATE_9600);
}

void loop()
{
    task();
}

void task(void)
{
    readHC06Msg();
    uint8_t value;
    switch (iping_state) {
    case IDLE:
        break;
    case INIT:
        iping_state = READY;
        break;
    case READY:
        if (CHK_BIT(msg_flags, RXCMD_A)) {
            CLR_BIT(msg_flags, RXCMD_A);
            value = atoi(message[RXCMD_A].data);
            Serial.print("A = ");
            Serial.println(value);
        }
        if (CHK_BIT(msg_flags, RXCMD_B)) {
            CLR_BIT(msg_flags, RXCMD_B);
            value = atoi(message[RXCMD_B].data);
            Serial.print("B = ");
            Serial.println(value);
        }
        if (CHK_BIT(msg_flags, RXCMD_C)) {
            CLR_BIT(msg_flags, RXCMD_C);
            value = atoi(message[RXCMD_C].data);
            Serial.print("C = ");
            Serial.println(value);
        }
        if (CHK_BIT(msg_flags, RXCMD_D)) {
            CLR_BIT(msg_flags, RXCMD_D);
            value = atoi(message[RXCMD_D].data);
            Serial.print("D = ");
            Serial.println(value);
        }
        if (CHK_BIT(msg_flags, RXCMD_R)) {
            CLR_BIT(msg_flags, RXCMD_R);
            value = atoi(message[RXCMD_R].data);
            Serial.print("R = ");
            Serial.println(value);
        }
        if (CHK_BIT(msg_flags, RXCMD_S)) {
            CLR_BIT(msg_flags, RXCMD_S);
            value = atoi(message[RXCMD_S].data);
            Serial.print("S = ");
            Serial.println(value);
        }
        if (CHK_BIT(msg_flags, RXCMD_E)) {
            CLR_BIT(msg_flags, RXCMD_E);
            value = atoi(message[RXCMD_E].data);
            Serial.print("E = ");
            Serial.println(value);
        }
        break;
    default:
        break;
    }
}
