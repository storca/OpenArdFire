#include "RFID.h"

RFID::RFID(SoftwareSerial *rfid)
{
  this->_rfid = new RDM6300<SoftwareSerial> rdm(rfid);
}
RFID::RFID(HardwareSerial *rfid)
{
  this->_rfid = new RDM6300<HardwareSerial> rdm(rfid);
}
void RFID::addTags(long tags[])
{
  this->_tags = tags;
  this->_numberOfTags = sizeof(*this->_tags) / sizeof(long);
}
void RFID::handler()
{
  long last_id;
  last_id = this->_rfid->read();
  for(int i=0; i<=this->_numberOfTags, i++)
  {
    if(last_id == this->_tags[i])
    {
      //Invert RFID state
      this->_rfidState = !this->_rfidState;
    }
  }
}
bool RFID::valid()
{
  return this->_rfidState;
}
RFID::~RFID()
{
  delete this->_rfid;
}
