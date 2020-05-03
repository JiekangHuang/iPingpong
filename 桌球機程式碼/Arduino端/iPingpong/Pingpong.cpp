#include "BT.h"

BT::BT(uint8_t rx_pin, uint8_t tx_pin) : SoftwareSerial(rx_pin, tx_pin)
{
}

BT::~BT()
{
}

bool BT::init(uint32_t baudrate)
{
    this->begin(baudrate);
    return true;
}

bool BT::snedCommand(char *cmd)
{
    if (cmd == NULL) {
        return false;
    }
    this->println(cmd);
    return true;
}

bool BT::receiveCommand(void)
{
    if (this->available()) {
        strcat(this->buff, this->readStringUntil('\n').c_str());
    }
    return true;
}