#include "BT.h"

#define BT_RX 2
#define BT_TX 3

Pingpong my_bt(BT_RX, BT_TX);

long pre_time = 0;

void setup()
{
    Serial.begin(BAUDRATE_115200);
    my_bt.init(BAUDRATE_115200);
    pre_time = millis();
}

void loop()
{
    my_bt.task();
    Serial.println("A");
    if (millis() - pre_time >= 1000) {
        pre_time = millis();
        Serial.println("B");
    }
    Serial.println("C");
}