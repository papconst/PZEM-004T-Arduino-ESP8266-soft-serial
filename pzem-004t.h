#ifndef PZEM_004T_H
#define PZEM_004T_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SoftwareSerial.h>
#include <IPAddress.h>

#define baudRate 9600

#define CMD_DELAY 1000

#define MSG_LEN 7
#define MSG_ERR -1

#define req_V    0xB0
#define req_I    0xB1
#define req_P    0xB2
#define req_E    0xB3
#define req_Addr 0xB4
#define req_Ia   0xB5

class PZEM_004T
{
public:
    PZEM_004T(const uint8_t pinRx, const uint8_t pinTx);
    PZEM_004T( *port);

    float getVoltage();
    float getCurrent();
    float getPower();
    float getEnergy();
    bool setAlarm(const uint8_t Ia);
    bool setAddress(const uint8_t a1 = 192,const uint8_t a2 = 168,const uint8_t a3 = 1,const uint8_t a4 = 1);

private:
    Stream *swSer;
    void sendCommand(const uint8_t com, const uint8_t arg = 0x00);
    uint8_t getCrc(const uint8_t data[MSG_LEN]);
    bool checkCrc(const uint8_t data[MSG_LEN]);
    bool getData(uint8_t cmd, uint8_t arg, uint8_t *data);
};
#endif // PZEM_004T_H
