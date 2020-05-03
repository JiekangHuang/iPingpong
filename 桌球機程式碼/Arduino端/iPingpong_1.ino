#include <Servo.h>
Servo  servo1, servo2;

#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *Motor[] = {AFMS.getMotor(1), AFMS.getMotor(2), AFMS.getMotor(3), AFMS.getMotor(4)};


char mode = '1';
int RandomDelay = 500;
int motor_speedLv[] = {0, 0, 2, 2};
int speedo[] = {42, 84, 126, 168, 210, 255};
bool Random = false;
bool start = false;
unsigned long previousTime;



void setup() {
  Serial.begin(9600);
  AFMS.begin();
  servo1.attach(9);
  servo2.attach(10);
  Motor[0]->setSpeed(speedo[motor_speedLv[0]]);
  Motor[1]->setSpeed(speedo[motor_speedLv[1]]);
  Motor[2]->setSpeed(speedo[motor_speedLv[2]]);
  Motor[3]->setSpeed(speedo[motor_speedLv[3]]);
}

void loop() {
  char sign, val;
  String deg;

  if (Serial.available()) {
    sign = Serial.read();
    //Serial.println(sign);
    delay(5);

    switch (sign) {

      case 'A':
        val = Serial.read();
        mode = val;
        modeChange();
        break;

      case 'B':
        val = Serial.read();
        speedChange(val);
        break;

      case 'C':
        while (Serial.available()) {
          val = Serial.read();
          if (val == 'C' || val == 'D')break;
          deg += val;
        }
        servo1.write(deg.toInt());
        Serial.println(deg);
        break;

      case 'D':
        while (Serial.available()) {
          val = Serial.read();
          if (val == 'C' || val == 'D')break;
          deg += val;
        }
        servo2.write(deg.toInt());
        Serial.println(deg);
        break;

      case 'R':
        val = Serial.read();
        if (val == '1') {
          Serial.println("Random On");
          Random = true;
          previousTime = millis();
        } else  {
          Serial.println("Random Off");
          Random = false;
        }
        break;

      case 'S':
        val = Serial.read();
        if (val == '0') {
          Serial.println("STOP");
          start = false;
          for (int i = 0 ; i < 4; i++)Motor[i]->run(RELEASE);
        } else  {
          Serial.println("START");
          start = true;
          modeChange();
        }
        break;

      case 'Q':
        Serial.println("STOP");
        for (int i = 0 ; i < 4; i++)Motor[i]->run(RELEASE);
        start = false;
        break;
    }
  }

  if (Random && start) {
    int passedTime = millis() - previousTime;
    if (passedTime > RandomDelay) {
      int num = random(15, 175);
      Serial.print("Random:");
      Serial.println(num);
      servo1.write(num);
      previousTime = millis();
    }
  }

}



void modeChange() {
  if (start) {
    switch (mode) {
      case '1':
        Serial.println("Mode:1");
        Motor[0]->run(FORWARD);
        Motor[1]->run(RELEASE);
        break;
      case '2':
        Serial.println("Mode:2");
        Motor[0]->run(RELEASE);
        Motor[1]->run(FORWARD);
        break;
      case '3':
        Serial.println("Mode:3");
        Motor[0]->run(FORWARD);
        Motor[1]->run(FORWARD);
        break;
    }
    Motor[2]->run(FORWARD);
    Motor[3]->run(FORWARD);
  }
}

void speedChange(char i) {
  String s;
  s = i;
  motor_speedLv[0] = s.toInt() - 1;
  motor_speedLv[1] = s.toInt() - 1;
  Motor[0]->setSpeed(speedo[motor_speedLv[0]]);
  Motor[1]->setSpeed(speedo[motor_speedLv[1]]);
  Serial.print("Speed:");
  Serial.println(speedo[motor_speedLv[0]]);
}
