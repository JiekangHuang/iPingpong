#include "HC06.h"
#include "config.h"
#include <SoftwareSerial.h>

SoftwareSerial BT(BT_RX, BT_TX);

extern uint32_t msg_flags;

extern char               A[4];
extern char               B[4];
extern char               C[4];
extern char               D[4];
extern char               E[4];
extern char               F[4];

int initHC06(uint8_t rx_pin, uint8_t tx_pin, uint32_t baudrate)
{
    if (rx_pin < 2 || tx_pin < 2) {
        return ERROR_HC06_RXTX_PIN;
    }
    if (baudrate != BAUDRATE_9600 && baudrate != BAUDRATE_38400 && baudrate != BAUDRATE_115200) {
        return ERROR_HC06_BAUDRATE;
    }
    BT.begin(baudrate);
    return ERROR_HC06_NONE;
}

int readHC06Msg(void)
{
    static char buffer[32], *data = NULL;
    static int  len = 0, cr = false, state = RX_HEADER, ii = 0;
    char        c;

    if (BT.available()) {
        switch (state) {
        default:
        case RX_HEADER:
            /* get header CR*/
            c = BT.read();
#if READ_DEBUG
            Serial.print(c);
#endif
            if (c == '\r') {
                cr    = true;
            } else if (c == '\n') {
                if (cr) {
                    cr = false;
                    state = RX_CONTENT;
                } else {
                    /* error */
                    state = RX_ERROR;
                }
            }
            break;
        case RX_CONTENT:
            /* get content */
            buffer[len] = BT.read();
#if READ_DEBUG
            Serial.print(buffer[len]);
#endif
            if (buffer[len] == '\r') {
                cr          = true;
                buffer[len] = '\0';
                state       = RX_FOOTER;
            } else {
                for (ii = 0; message[ii].command != NULL; ii++) {
                    if (message[ii].type != MSG_NONE) {
                        if (strcmp(message[ii].command, buffer) == 0) {
                            /* got it */
                            data = message[ii].data;
                            if (data != NULL) {
                                state = RX_DATA;
                            }
                            break;
                        }
                    }
                }
                if (message[ii].command == NULL) {
                    // command not found
                    state = RX_ERROR;
                }
                len = 0;
            }
            break;
        case RX_DATA:
            data[len] = BT.read();
#if READ_DEBUG
            Serial.print(data[len]);
#endif
            if (data[len] == '\r') {
                cr        = true;
                data[len] = '\0';
                state     = RX_FOOTER;
            } else {
                // normal character
                len++;
            }
            break;
        case RX_FOOTER:
            /* get footer CR/LF */
            c = BT.read();
#if READ_DEBUG
            Serial.print(c);
#endif
            if (c == '\n') {
                if (cr) {
                    cr    = false;
                    len   = 0;
                    state = RX_HEADER;
                    data  = NULL;
                    // raise flag up.
                    for (ii = 0; message[ii].command != NULL; ii++) {
                        if (message[ii].type == MSG_COLON) {
                            if (strcmp(message[ii].command, buffer) == 0) {
                                /* got it */
                                SET_BIT(msg_flags, ii);
                                break;
                            }
                        }
                    }
                }
            } else {
                /* error */
                state = RX_ERROR;
            }
            break;
        case RX_ERROR:
            /* error state, flush all data */
            c = BT.read();     // dummy read
#if READ_DEBUG
            Serial.print(c);
#endif
            if (c == '\n') {
                cr    = false;
                len   = 0;
                state = RX_HEADER;
                data  = NULL;
            }
            break;
        }
    }

    if (len) {
        return len;
    } else {
        return ERROR_HC06_NO_DATA;
    }
}