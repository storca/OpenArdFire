#include "CommunicationHandler.h"

CommunicationHandler::CommunicationHandler(HardwareSerial *s, SoftwareSerial *rf, unsigned int deviceAddress)
{
  this->_localSerial = s;
  this->_rfSerial = rf;
  this->_deviceAddress = deviceAddress;
  this->_messageHandler = new Message(&this->_deviceAddress);
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
/**
 * Handles local serial communication
 */
void CommunicationHandler::localSerialHandler()
{
  //Read from serial
  if(this->_localSerial->available())
  {
    char character = this->_localSerial->read();
    if(character == CH_ENDL_CHAR)
    {
      this->_localSerialMessage += '\n';
      this->processMessage(this->_localSerialMessage, Local);
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
    //No need to encode messages here
    this->_rfSerial->print(this->_localSerialBuffer);
    this->_localSerialBuffer = "";
  }
}
/**
 * Handles RF serial communication
 */
void CommunicationHandler::rfSerialHandler()
{
  //Read and process from serial
  if(this->_rfSerial->available())
  {
    char character = this->_rfSerial->read();
    if(character == CH_ENDL_CHAR)
    {
      this->_rfSerialMessage += '\n';
      this->processMessage(this->_rfSerialMessage, Radio);
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
    //Encode message before sending
    this->_rfSerial->print(processSend(&_rfSerialBuffer));
    //No need to empty buffer here as soon as the command are deleted one by one when they are sended
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
/**
 * Process a message when received
 * @param message Message to process
 * @param from    What serial
 */
void CommunicationHandler::processMessage(String message, int from)
{
  _messageHandler->setMessage(message);
  if(_messageHandler->msg->from = "m" && isAddressInt(_messageHandler->msg->to))
  {
    if(_messageHandler->msg->to.toInt() == _deviceAddress || _messageHandler->msg->to == "a")
    {
      switch (from) {
        case Local:
          _localProcessQueue + _messageHandler->msg->command;
        case Radio:
          _rfProcessQueue + _messageHandler->msg->command;
      }
    }
  }
}
/**
 * Returns the a single command from the buffer
 * @param  from Local or Radio
 * @return      A command with it's arguments with \n at the end
 */
String CommunicationHandler::getCommand(String *commandQueue)
{
  //Function tested and fully working
  //The return result
  String command;

  int i = 0;
  while((*commandQueue)[i] != '\n')
  {
    i++;
  }
  //Skip the character \n
  i++;

  command = commandQueue->substring(0, i);
  commandQueue->remove(0, i);
  return command;
}
/**
 * Get a command from a device
 * @param  from Local or Radio
 * @return      A command from a device (unencoded)
 */
String CommunicationHandler::getCommand(int from)
{
  switch (from) {
    case Local:
      return getCommand(&this->_localProcessQueue);
    break;
    case Radio:
      return getCommand(&this->_rfProcessQueue);
    break;
  }
}
/**
 * Encodes and return a single command from a queue
 * @param  queue Queue to process
 * @return       Encoded command from cue
 */
String CommunicationHandler::processSend(String *queue)
{
  Message *msg;
  String command = getCommand(queue);

  msg = new Message(&_deviceAddress);
  msg->setMessage(command);

  return msg->encodeMessage(command, msg->msg->to);
}

/**
 * Checks wether an address is an int or not
 * @param  address Address to be checked
 * @return         true if address is an int
 */
bool CommunicationHandler::isAddressInt(String address)
{
  //Tested and working
  unsigned int i = 0;
  while(i <= address.length())
  {
    if(!isDigit(address[i]) && !address[i] == '\0')
    {
      return false;
    }
    i++;
  }
  return true;
}
CommunicationHandler::~CommunicationHandler()
{
  delete _messageHandler;
}

/*
Message Class
 */
/**
 * New message object
 * @param msg Message to process
 */
Message::Message(unsigned int *deviceAddress)
{
  _deviceAddress = deviceAddress;
  msg = new message;
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
 * Encode a message with the protocol
 * @param  command  Command to send with its arguments
 * @param  receiver Receiver address
 * @return          Encoded message
 */
String Message::encodeMessage(String command, String receiver)
{
  String result = "";

  //Add our sender address
  result + String(*_deviceAddress);
  //Add separation char
  result += '/';
  //Add receiver address
  result + receiver;
  //Add separation char
  result += ':';
  //Add command
  result + command;
  //Add endline char
  result += '\n';

  return result;
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
    msg->from += _msgToProcess[i];
    i++;

    if(i >= maxMsgLen)
    {
      msg->from = "\0";
      return;
    }
  }

  //Skip the '/' character
  i++;

  while(_msgToProcess[i] != ':')
  {
    msg->to += _msgToProcess[i];
    i++;

    if(i >= maxMsgLen)
    {
      msg->to = "\0";
      return;
    }
  }

  //Skip the ':' character
  i++;

  //Rest of the string is the command
  while(_msgToProcess[i] != '\0')
  {
    msg->command += _msgToProcess[i];
    if(i >= maxMsgLen)
    {
      //message too long
      return;
    }
  }
}
Message::~Message()
{
  delete msg;
}
