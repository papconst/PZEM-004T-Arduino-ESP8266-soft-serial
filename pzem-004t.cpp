#include "pzem-004t.h"


IPAddress ip;

PZEM_004T::PZEM_004T(const uint8_t pinRx, const uint8_t pinTx)
{
  SoftwareSerial *port = new SoftwareSerial(pinRx, pinTx);
  port->begin(baudRate);
  this->swSer = port;
}

float PZEM_004T::getVoltage()
{
  uint8_t data[MSG_LEN];
  if(getData(req_V,0x00,data)) return (data[1] << 8) + data[2] + (data[3] / 10.0);
  else return MSG_ERR;
}

float PZEM_004T::getCurrent()
{
  uint8_t data[MSG_LEN];
  if(getData(req_I,0x00,data)) return data[2] + (data[3] / 100.0);
  else return MSG_ERR;
}

float PZEM_004T::getPower()
{
  uint8_t data[MSG_LEN];
  if(getData(req_P,0x00,data)) return (data[1] << 8) + data[2];
  else return MSG_ERR;
}

float PZEM_004T::getEnergy()
{
  uint8_t data[MSG_LEN];
  if(getData(req_E,0x00,data)) return ((uint32_t)data[1] << 16) + ((uint16_t)data[2] << 8) + data[3];
  else return MSG_ERR;
}

bool PZEM_004T::setAddress(const uint8_t a1,const uint8_t a2,const uint8_t a3,const uint8_t a4)
{
  ip = IPAddress(a1,a2,a3,a4);
  uint8_t data[MSG_LEN];
  getData(req_Addr,0x00,data);
  if(data[MSG_LEN-1] == 0xA4) return true;// if address set ok, response is A4 00 00 00 00 00 A4, so checksum check is enough
  else return false;
}

bool PZEM_004T::setAlarm(const uint8_t Ia)
{
  if((Ia>=1) && (Ia <= 22))
  {
    uint8_t data[MSG_LEN];
    getData(req_Ia,Ia,data);
    if(data[MSG_LEN-1] == 0xA5) return true;// if alarm set ok, response is A5 00 00 00 00 00 A5, so checksum check is enough
    else return false;
  }
  else return false;
}

bool PZEM_004T::getData(uint8_t cmd, uint8_t arg, uint8_t *data)
{
  sendCommand(cmd,arg);
  uint8_t i = 0;
  while (swSer->available() > 0)
  {
    uint8_t incomingByte = swSer->read();
    data[i++] = incomingByte;
  } 
  if(checkCrc(data)) return true;
  else return false;
}

void PZEM_004T::sendCommand(const uint8_t com, const uint8_t arg)
{//arg is not null only if we are setting alarm treshold/
 // default params cant be set on esp8266
  uint8_t command[MSG_LEN];
  command[0] = com;
  for(uint8_t i = 0; i < 4; i++) command[i+1] = ip[i];
  command[5] = arg;
  command[6] = getCrc(command);
  swSer->write(command, sizeof(command));  
  yield();
  delay(CMD_DELAY);
}

uint8_t PZEM_004T::getCrc(const uint8_t data[MSG_LEN - 1])
{
  uint16_t crc = 0;
  for(uint8_t i = 0; i < MSG_LEN - 1; i++)
    crc += data[i];
  return (uint8_t)(crc & 0xFF); 
}

bool PZEM_004T::checkCrc(const uint8_t data[MSG_LEN])
{
  uint16_t crc = 0;
  for(int i = 0; i < MSG_LEN - 1 ; i++)// last uint8_t is a checksum and not used here
    crc += data[i];
  crc = (uint8_t)(crc & 0xFF);
  if(crc == data[MSG_LEN - 1]) return true;
  else return false;  
}
