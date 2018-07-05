#include "RFID.h"

/**
 * Create a new RFID handler
 * @param rfid RFID serial
 * @param fm   Firing Module
 */
RFID::RFID(SoftwareSerial *rfid, FiringModule *fm)
{
  this->_rfid = new RDM6300<SoftwareSerial>(rfid);
}

/**
 * Handles RFID
 */
void RFID::handler()
{
  unsigned long long last_id;
  last_id = this->_rfid->read();
  for(size_t i=0; i<this->_numberOfTags; i++)
  {
    if(last_id == this->_tags[i])
    {
      //Invert RFID state
      this->_rfidState = true;
    }
    else
    {
      this->_rfidState = false;
      //TODO : Uncomment when "emit()" ready
      //_fm->emit(String("invalidtag ") + String(last_id, HEX));
    }
  }
}
/**
 * Add some tags
 * @param tags tags to add
 */
void RFID::addTags(unsigned long long *tags)
{
  _tags = tags;
}
/**
 * Add a single tag to the others
 * @param tag tag to add
 */
void RFID::addTag(unsigned long long *tag)
{
  unsigned long long *newTags;
  _numberOfTags = (sizeof(*_tags) / sizeof(unsigned long long)) + 1;
  newTags = new unsigned long long[_numberOfTags];
  //TODO : copy _tags in newTags
  delete _tags;
  _tags = newTags;
}
/**
 * Checks if the RFID is valid
 * @return true if valid
 */
bool RFID::valid()
{
  return this->_rfidState;
}
RFID::~RFID()
{
  delete this->_rfid;
  delete _tags;
}
