#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>

#ifndef Pingpong_H
#define Pingpong_H

#define BAUDRATE_9600 9600
#define BAUDRATE_19200 19200
#define BAUDRATE_115200 115200

class Pingpong : public SoftwareSerial {
  private:
    char buff[255];
    bool snedCommand(char *cmd);
    bool receiveCommand(void);
    bool shootBall(uint8_t speed);
    bool recycleBall(uint8_t speed);
    bool setBodyDir(uint8_t angle);
    bool setShootAngle(uint8_t angle);
    bool setBallPath(bool bapa);

  public:
    Pingpong(uint8_t rx_pin, uint8_t tx_pin);
    ~Pingpong();
    bool init(uint32_t baudrate);
    bool task(void);
};

#endif
