#include "HC06.h"
#include "config.h"

#define BT Serial

extern uint32_t msg_flags;

extern char A[4];
extern char B[4];
extern char C[4];
extern char D[4];
extern char E[4];
extern char R[4];
extern char S[4];

int initHC06(uint32_t baudrate)
{
    BT.begin(baudrate);
    return ERROR_HC06_NONE;
}

int readHC06Msg(void)
{
    static char buffer[32], *data = NULL;
    static int  len = 0, cr = false, type = MSG_NONE, state = RX_HEADER, ii = 0;
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
                cr = true;
            } else if (c == '\n') {
                if (cr) {
                    cr    = false;
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
                type        = MSG_NONE;
                cr          = true;
                buffer[len] = '\0';
                state       = RX_FOOTER;
            } else if (buffer[len] == ':') {
                type        = MSG_COLON;
                buffer[len] = '\0';
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
            } else {
                // normal character
                len++;
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
                    if (type == MSG_NONE) {
                        // command only (Q)
                        for (ii = 0; message[ii].command != NULL; ii++) {
                            if (message[ii].type == MSG_NONE) {
                                if (strcmp(message[ii].command, buffer) == 0) {
                                    /* got it */
                                    SET_BIT(msg_flags, ii);
                                    break;
                                }
                            }
                        }
                        if (message[ii].command == NULL) {
                            // command not found
                            state = RX_ERROR;
                        }
                    } else if (type == MSG_COLON) {
                        SET_BIT(msg_flags, ii);
                    } else {
                        // shoud not be here
                    }
                    type = MSG_NONE;
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