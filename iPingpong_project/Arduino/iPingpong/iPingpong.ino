#include "HC06.h"
#include "config.h"
#include <Adafruit_MotorShield.h>
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

Adafruit_MotorShield AFMS    = Adafruit_MotorShield();
Adafruit_DCMotor *   motors[MOTOR_MAX] = {AFMS.getMotor(3), AFMS.getMotor(4)};

Servo servo_up;
Servo servo_down;
Servo servo_receive_ball;

uint32_t msg_flags;
uint8_t ball_mode;

// FIXME: RENAME
char A[4];
char B[4];
char C[4];
char D[4];
char E[4];
char R[4];
char S[4];

const rx_message_t message[RXCMD_MAX + 1] = {{"A", 1, MSG_COLON, A}, {"B", 1, MSG_COLON, B}, {"C", 1, MSG_COLON, C}, {"D", 1, MSG_COLON, D},
                                            {"E", 1, MSG_COLON, E}, {"Q", 1, MSG_NONE, NULL}, {"R", 1, MSG_COLON, R}, {"S", 1, MSG_COLON, S},
                                            {NULL, 0, MSG_NONE, NULL}};

uint8_t iping_state = INIT;
void    task(void);
void    updateBallMode(uint8_t mode);

void setup()
{
#if DEBUG_MODE
    debugSerial.begin(BAUDRATE_115200);
#endif
    initHC06(BT_RX, BT_TX, BAUDRATE_9600);
    servo_up.attach(10);
    servo_down.attach(9);
    servo_receive_ball.attach(8);
    AFMS.begin();
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
        msg_flags = 0;
        ball_mode = 0;
        servo_up.write(DEFAULT_SERVO_UP_VALUE);
        servo_down.write(DEFAULT_SERVO_DOWN_VALUE);

        for (int ii = 0; ii < MOTOR_MAX; ii++) {
            motors[ii]->run(RELEASE);
        }
        iping_state = READY;
        break;
    case READY:
        // 上下旋球
        if (CHK_BIT(msg_flags, RXCMD_A)) {
            CLR_BIT(msg_flags, RXCMD_A);
            value = atoi(message[RXCMD_A].data);
            DEBUG_PRINT("A = ");
            DEBUG_PRINTLN(value);
            ball_mode = value;
        }
        // 球速
        if (CHK_BIT(msg_flags, RXCMD_B)) {
            CLR_BIT(msg_flags, RXCMD_B);
            value = map(atoi(message[RXCMD_B].data), 1, 6, 20, 100);
            DEBUG_PRINT("B = ");
            DEBUG_PRINTLN(value);
            for (int ii = 0; ii < MOTOR_MAX; ii++) {
                motors[ii]->setSpeed(value);
            }
        }
        // 擺動角度
        if (CHK_BIT(msg_flags, RXCMD_C)) {
            CLR_BIT(msg_flags, RXCMD_C);
            value = atoi(message[RXCMD_C].data);
            DEBUG_PRINT("C = ");
            DEBUG_PRINTLN(value);
            servo_down.write(value);
        }
        // 側旋轉
        if (CHK_BIT(msg_flags, RXCMD_D)) {
            CLR_BIT(msg_flags, RXCMD_D);
            value = map(atoi(message[RXCMD_D].data), 0, 270, 0, 180);
            DEBUG_PRINT("D = ");
            DEBUG_PRINTLN(value);
            servo_up.write(value);
        }
        // 隨機擺動
        if (CHK_BIT(msg_flags, RXCMD_R)) {
            CLR_BIT(msg_flags, RXCMD_R);
            value = atoi(message[RXCMD_R].data);
            DEBUG_PRINT("R = ");
            DEBUG_PRINTLN(value);
        }
        // 啟動/停止
        if (CHK_BIT(msg_flags, RXCMD_S)) {
            CLR_BIT(msg_flags, RXCMD_S);
            value = atoi(message[RXCMD_S].data);
            DEBUG_PRINT("S = ");
            DEBUG_PRINTLN(value);
            if ((bool)value) {
                updateBallMode(ball_mode);
            } else {
                motors[0]->run(RELEASE);
                motors[1]->run(RELEASE);
            }
        }
        // 收球控制
        if (CHK_BIT(msg_flags, RXCMD_E)) {
            CLR_BIT(msg_flags, RXCMD_E);
            value = atoi(message[RXCMD_E].data);
            DEBUG_PRINT("E = ");
            DEBUG_PRINTLN(value);
            if ((bool)value) {
                servo_receive_ball.write(84);
            } else {
                servo_receive_ball.write(90);
            }
        }
        // 斷線控制
        if (CHK_BIT(msg_flags, RXCMD_Q)) {
            CLR_BIT(msg_flags, RXCMD_Q);
            DEBUG_PRINTLN("Q");
            iping_state = INIT;
        }
        break;
    default:
        break;
    }
}

void updateBallMode(uint8_t mode) 
{
    if(mode == 1) {
        motors[0]->run(FORWARD);
        motors[1]->run(RELEASE);
    } else if(mode == 2) {
        motors[0]->run(RELEASE);
        motors[1]->run(FORWARD);
    } else if(mode == 3) {
        motors[0]->run(FORWARD);
        motors[1]->run(FORWARD);
    }
}
