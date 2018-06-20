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
  if(this->_localSerial->available())
  {
    char character = this->_localSerial->read();
    if(character == '\n')
    {
      this->processCommand(this->_localSerialMessage, LOCAL);
      this->_localSerialMessage = "";
    }
    else
    {
      this->_localSerialMessage += character;
    }
  }
}
void CommunicationHandler::rfSerialHandler()
{
  if(this->_rfSerial->available())
  {
    char character = this->_rfSerial->read();
    if(character == '\n')
    {
      this->processCommand(this->_rfSerialMessage, RF);
      this->_rfSerialMessage = "";
    }
    else
    {
      this->_rfSerialMessage += character;
    }
  }
}

void CommunicationHandler::processCommand(String cmd, int from)
{
  if(cmd[0] == 'm' && cmd[1] == '/')
  {
    /*
    This is a simple message to the transmitter
    So we do nothing
     */
  }
  else
  {
    if(this->finddeviceAddress(cmd) == int(this->_deviceAddress))
    {
      /*
      The message is for us !
       */

    }
  }
}


/**
 * Extract a command from a String
 * @param  cmd command eg "1154/do something" or "do something"
 * @return     extracted command eg "do something"
 */
String CommunicationHandler::extractCommand(String cmd)
{
  int i = 0;

  int length = cmd.length();

  //Skipping the numbers
  while(cmd[i] != '/')
  {
    i++;

    /*
      If this is resptected, then, there is no '/' in the command
      Note : this ommits the last character
      TO OPTIMISE TO MAKE IT WORK WITH CH_MAX_DEVICE_ADDRESS_LEN
     */
    if(i > length)
    {
      return cmd;
    }
  }

  //Skip the '/' character
  i++;

  String buffer;
  for(int k=i; k<=length; k++)
  {
    buffer += cmd[k];
  }
  return buffer;
}

int CommunicationHandler::finddeviceAddress(String s)
{
  /*
  Command form : <device_index>/<command>
  if device_index is 'm' then no receiver should interpret it
  if the command is like this : "do something", then it will return 0 as a device index
   */

   if(s[0] == '/')
   {
     //Avoid crash due to toInt() conversion with a letter
     if(s[1] == 'm')
     {
       return -1;
     }
     return 0;
   }


  String buffer = "";

  for(int i=0; (s[i] != '/') && (i < (CH_MAX_DEVICE_ADDRESS_LEN+1) ); i++)
  {
    //Add the device index to the buffer
    buffer += s[i];

    //Fix to avoid character or control conversion -> crash
    if(!isDigit(s[i]))
    {
      this->_log->log("Device address is not a digit", ERR);
      return -1;
    }


    if(i == CH_MAX_DEVICE_ADDRESS_LEN)
    {
      this->_log->log("No device address specified, assuming 0", DEBUG);
      return 0;
    }
  }

  //Convert and return device index
  return buffer.toInt();


}

CommunicationHandler::~CommunicationHandler()
{

}
