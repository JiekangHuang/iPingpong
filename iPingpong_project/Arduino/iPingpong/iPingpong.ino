#include "HC06.h"
#include "config.h"
#include <Arduino.h>

uint32_t msg_flags;

// FIXME: RENAME
char A[4];
char B[4];
char C[4];
char D[4];
char E[4];
char F[4];

const rx_message_t message[7] = {{"A", 1, MSG_COLON, A},   {"B", 1, MSG_COLON, B},
                                 {"C", 1, MSG_COLON, C},   {"D", 1, MSG_COLON, D},
                                 {"E", 1, MSG_COLON, E},   {"F", 1, MSG_COLON, F},
                                 {NULL, 0, MSG_NONE, NULL}};

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
}

void task(void)
{
    readHC06Msg();
    uint8_t value;
    switch (iping_state) {
    case IDLE:
        break;
    case INIT:
        break;
    case READY:
        if (CHK_BIT(msg_flags, RXCMD_A)) {
            CLR_BIT(msg_flags, RXCMD_A);
            value = atoi(message[RXCMD_A].data);
        }
        if (CHK_BIT(msg_flags, RXCMD_B)) {
            CLR_BIT(msg_flags, RXCMD_B);
            value = atoi(message[RXCMD_B].data);
        }
        if (CHK_BIT(msg_flags, RXCMD_C)) {
            CLR_BIT(msg_flags, RXCMD_C);
            value = atoi(message[RXCMD_C].data);
        }
        if (CHK_BIT(msg_flags, RXCMD_D)) {
            CLR_BIT(msg_flags, RXCMD_D);
            value = atoi(message[RXCMD_D].data);
        }
        if (CHK_BIT(msg_flags, RXCMD_E)) {
            CLR_BIT(msg_flags, RXCMD_E);
            value = atoi(message[RXCMD_E].data);
        }
        if (CHK_BIT(msg_flags, RXCMD_F)) {
            CLR_BIT(msg_flags, RXCMD_F);
            value = atoi(message[RXCMD_F].data);
        }
        break;
    default:
        break;
    }
}
