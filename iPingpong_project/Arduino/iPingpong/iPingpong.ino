#include "HC06.h"
#include "config.h"
#include <Adafruit_MotorShield.h>
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

Adafruit_MotorShield AFMS    = Adafruit_MotorShield();
Adafruit_DCMotor *   Motor[] = {AFMS.getMotor(3), AFMS.getMotor(4)};

Servo servo_up;
Servo servo_down;
Servo servo_receive_ball;

uint32_t msg_flags;

// FIXME: RENAME
char A[4];
char B[4];
char C[4];
char D[4];
char R[4];
char S[4];
char E[4];

const rx_message_t message[8] = {{"A", 1, MSG_COLON, A}, {"B", 1, MSG_COLON, B},
                                 {"C", 1, MSG_COLON, C}, {"D", 1, MSG_COLON, D},
                                 {"R", 1, MSG_COLON, R}, {"S", 1, MSG_COLON, S},
                                 {"E", 1, MSG_COLON, E}, {NULL, 0, MSG_NONE, NULL}};

uint8_t iping_state = INIT;
void    task(void);

void setup()
{
#if DEBUG_MODE
    debugSerial.begin(BAUDRATE_9600);
#endif
    initHC06(BT_RX, BT_TX, BAUDRATE_9600);
    servo_up.attach(10);
    servo_down.attach(9);
    servo_receive_ball.attach(8);
    AFMS.begin();

    Motor[0]->setSpeed(20);
    Motor[1]->setSpeed(20);
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
        // 上下旋球
        if (CHK_BIT(msg_flags, RXCMD_A)) {
            CLR_BIT(msg_flags, RXCMD_A);
            value = atoi(message[RXCMD_A].data);
            Serial.print("A = ");
            Serial.println(value);

            if (value == 1) {
                Motor[0]->run(RELEASE);
                Motor[1]->run(FORWARD);
            } else if (value == 2) {
                Motor[0]->run(FORWARD);
                Motor[1]->run(RELEASE);
            } else {
                Motor[0]->run(FORWARD);
                Motor[1]->run(FORWARD);
            }
        }
        // 球速
        if (CHK_BIT(msg_flags, RXCMD_B)) {
            CLR_BIT(msg_flags, RXCMD_B);
            value = map(atoi(message[RXCMD_B].data), 1, 6, 20, 100);
            Serial.print("B = ");
            Serial.println(value);
            Motor[0]->setSpeed(value);
            Motor[1]->setSpeed(value);
        }
        // 擺動角度
        if (CHK_BIT(msg_flags, RXCMD_C)) {
            CLR_BIT(msg_flags, RXCMD_C);
            value = atoi(message[RXCMD_C].data);
            value = map(atoi(message[RXCMD_D].data), 15, 175, 57, 117);
            Serial.print("C = ");
            Serial.println(value);
            servo_down.write(value);
        }
        // 側旋轉
        if (CHK_BIT(msg_flags, RXCMD_D)) {
            CLR_BIT(msg_flags, RXCMD_D);
            value = map(atoi(message[RXCMD_D].data), 0, 270, 0, 180);
            Serial.print("D = ");
            Serial.println(value);
            servo_up.write(value);
        }
        // 隨機擺動
        if (CHK_BIT(msg_flags, RXCMD_R)) {
            CLR_BIT(msg_flags, RXCMD_R);
            value = atoi(message[RXCMD_R].data);
            Serial.print("R = ");
            Serial.println(value);
        }
        // 啟動/停止
        if (CHK_BIT(msg_flags, RXCMD_S)) {
            CLR_BIT(msg_flags, RXCMD_S);
            value = atoi(message[RXCMD_S].data);
            Serial.print("S = ");
            Serial.println(value);
            if ((bool)value) {
                Motor[0]->run(FORWARD);
                Motor[1]->run(FORWARD);
            } else {
                Motor[0]->run(RELEASE);
                Motor[1]->run(RELEASE);
            }
        }
        // 收球控制
        if (CHK_BIT(msg_flags, RXCMD_E)) {
            CLR_BIT(msg_flags, RXCMD_E);
            value = atoi(message[RXCMD_E].data);
            Serial.print("E = ");
            Serial.println(value);
            if ((bool)value) {
                servo_receive_ball.write(84);
            } else {
                servo_receive_ball.write(90);
            }
        }
        break;
    default:
        break;
    }
}
