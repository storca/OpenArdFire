#include "RFID.h"

RFID::RFID(SoftwareSerial *rfid, long baudrate)
{
  this->_rfids = rfid;

  this->_rfids->begin(baudrate);
}
RFID::RFID(HardwareSerial *rfid, long baudrate)
{
  this->_rfidh = rfid;
}
void RFID::addTags(int tags[][14])
{
  this->_tags = tags;
}
void RFID::handler()
{

}
RFID::~RFID()
{

}
