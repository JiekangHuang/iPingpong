#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);

Servo servo1;

void setup()
{
  Serial.begin(115200);
  BT.begin(9600);
  servo1.attach(9);
}

void loop()
{
  if (Serial.available()) {
    String str = Serial.readStringUntil('\n');
    Serial.println(str);
    servo1.write(str.toInt());
  }
  if (BT.available()) {
    char ch = BT.read();
    Serial.println(ch);
  }
}
