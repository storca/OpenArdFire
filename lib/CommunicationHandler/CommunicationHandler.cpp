#include "CommunicationHandler.h"

CommunicationHandler::CommunicationHandler(HardwareSerial *s, SoftwareSerial *rf)
{
  this->_localSerial = s;
  this->_rfSerial = rf;

}
/**
 * Handles serial communications, call this function in loop
 */
void CommunicationHandler::handler()
{
  //Local Serial port
  if(this->_localSerial->available())
  {
    char character = this->_localSerial->read();
    if(character == '\n')
    {
      //processCommand(); to do
    }
    else
    {
      this->_localSerialMessage += character;
    }
  }

  //RF Serial ports
  if(this->_rfSerial->available())
  {
    char character = this->_rfSerial->read();
    if(character == '\n')
    {
      //processCommand(); to do
    }
    else
    {
      this->_rfSerialMessage += character;
    }
  }
}

CommunicationHandler::~CommunicationHandler()
{

}
