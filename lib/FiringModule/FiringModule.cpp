#include "FiringModule.h"

FiringModule::FiringModule(uint8_t address)
{
  _radio = new CRadio();
  _radio->begin(address);

  _mymsg = new Message(address);
}

/**
 * Set a security of the x securities defined
 * @param securityId Security Id eg. 0, 1, 2...
 * @param state      Status of the security (false -> enabled, true->disabled)
 */
void FiringModule::setSecurity(int securityId, bool state)
{
  this->_securities[securityId] = state;
}

/**
 * Checks if the firing module's securities are disabled
 * @return true->securities are disabled
 */
bool FiringModule::isSafe()
{
  for(unsigned int i=0; i<= sizeof(this->_securities); i++)
  {
    //If security is set to false
    if(!this->_securities[i])
    {
      return false;
    }
  }
  return true;
}

void FiringModule::sendErrorCode(int errorCode)
{
  String command = String("e ") + String(errorCode);
  //send to master
  //
  _radio->send(command, 0);
}

void FiringModule::processMessage(Message msg)
{
  String cmd;
  cmd = splitCommand(msg.msg.command, 0);

  if(cmd == "ping")
  {
    //Encode answer
    _mymsg->setCommand("pong", msg.msg.from);
    //Send it
    _radio->send(_mymsg);
  }
  else if(cmd == "info")
  {
    info();
  }
  else if(cmd == "check")
  {
    selfcheck();
  }
  else if(cmd == "test")
  {

  }
  else if(cmd == "security")
  {

  }
  else
  {
    sendErrorCode(CODE_COMMAND_NOT_UNDERSTOOD);
  }
}

String FiringModule::splitCommand(String command, int argument)
{
  String textToReturn = "";

  //Iterate over characters
  int i = 0;
  //Iterate over words
  int i2 = 0;

  int maxLength = command.length();

  while(i2 != argument)
  {
    textToReturn = "";
    while(command[i] != '\0' && command[i] != 32)
      {
        textToReturn += command[i];
        i++;
        if(i > maxLength)
        {
          return String("Command is too long");
        }
      }
      i++;
      //Skip the spaces
      while(command[i] == 32)
        {
          i++;
        }
      i2++;
  }
  return textToReturn;
}

FiringModule::~FiringModule()
{

}
