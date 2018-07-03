#include "CommunicationHandler.h"

CommunicationHandler::CommunicationHandler(HardwareSerial *s, SoftwareSerial *rf, Logger *log, unsigned int deviceAddress)
{
  this->_localSerial = s;
  this->_rfSerial = rf;

  this->_log = log;

  this->_deviceAddress = deviceAddress;
}
/**
 * Handles serial communications, call this function in loop
 */
void CommunicationHandler::handler()
{
  //Local Serial port
  this->localSerialHandler();

  //RF Serial port
  this->rfSerialHandler();

}

void CommunicationHandler::localSerialHandler()
{
  //Read from serial
  if(this->_localSerial->available())
  {
    char character = this->_localSerial->read();
    if(character == CH_ENDL_CHAR)
    {
      this->processCommand(this->_localSerialMessage, Local);
      this->_localSerialMessage = "";
    }
    else
    {
      this->_localSerialMessage += character;
    }
  }
  //Write to Serial
  if(needSend(&this->_localSerialBuffer) && !this->_localSerial->available())
  {
    this->_rfSerial->print(this->_localSerialBuffer);
    this->_localSerialBuffer = "";
  }
}
void CommunicationHandler::rfSerialHandler()
{
  //Read and process from serial
  if(this->_rfSerial->available())
  {
    char character = this->_rfSerial->read();
    if(character == CH_ENDL_CHAR)
    {
      this->processCommand(this->_rfSerialMessage, Radio);
      this->_rfSerialMessage = "";
    }
    else
    {
      this->_rfSerialMessage += character;
    }
  }
  //Send messages
  //NOTE : Avoid sending messages while receiving, might not work
  if(needSend(&this->_rfSerialBuffer) && !this->_rfSerial->available())
  {
    //print the buffer
    //TODO : Check buffer len before sending
    //TODO : Add destination address
    this->_rfSerial->print(this->_rfSerialBuffer);
    //empty buffer
    this->_rfSerialBuffer = "";
  }

}
/**
 * Send error code to specified Serial
 * @param errorCode error code
 * @param to        destination : Local, Radio, Both
 */
void CommunicationHandler::print(int errorCode, int to)
{
 switch (to) {
   case Local:
    this->_localSerialBuffer + String(errorCode) + CH_ENDL_CHAR;
   break;
   case Radio:
    this->_rfSerialBuffer + String(errorCode) + CH_ENDL_CHAR;
   break;
   case Both:
    this->_localSerialBuffer + String(errorCode) + CH_ENDL_CHAR;
    this->_rfSerialBuffer + String(errorCode) + CH_ENDL_CHAR;
   break;
 }
}
/**
 * Print a message to specified serial
 * @param msg message to send
 * @param to  destination : Local, Radio, Both
 */
void CommunicationHandler::print(const char *msg, int to)
{
  switch (to) {
    case Local:
      this->_localSerialBuffer + *msg;
    break;
    case Radio:
      this->_rfSerialBuffer + *msg;
    break;
    case Both:
      this->_localSerialBuffer + *msg;
      this->_rfSerialBuffer + *msg;
    break;
  }
}
/**
 * Checks if a buffer is empty
 * @param  buffer buffer as a pointer
 * @return        true if not empty
 */
bool CommunicationHandler::needSend(String *buffer)
{
  if(*buffer != "")
  {
    return true;
  }
  return false;
}
CommunicationHandler::~CommunicationHandler()
{

}

/*
Message Class
 */
/**
 * New message object
 * @param msg Message to process
 */
Message::Message(String msg)
{
  _message = new message;
  _msgToProcess = msg;
  process();
}
/**
 * Set message to process
 * @param msg Message to process
 */
void Message::setMessage(String msg)
{
  _msgToProcess = msg;
  process();
}
/**
 * Get message that has been processed
 * @return Message that has been processed
 */
String Message::getMessage()
{
  return _msgToProcess;
}
/**
 * Process the given message
 * @param maxMsgLen Max message length
 */
void Message::process(int maxMsgLen)
{
  int i = 0;
  while(_msgToProcess[i] != '/')
  {
    //Add character to from
    _message->from += _msgToProcess[i];
    i++;

    if(i >= maxMsgLen)
    {
      _message->from = "\0";
      return;
    }
  }

  //Skip the '/' character
  i++;

  while(_msgToProcess[i] != ':')
  {
    _message->to += _msgToProcess[i];
    i++;

    if(i >= maxMsgLen)
    {
      _message->to = "\0";
      return;
    }
  }

  //Skip the ':' character
  i++;

  //Rest of the string is the command
  while(_msgToProcess[i] != '\0')
  {
    _message->command += _msgToProcess[i];
    if(i >= maxMsgLen)
    {
      //message too long
      return;
    }
  }
}
Message::~Message()
{
  delete _message;
}
