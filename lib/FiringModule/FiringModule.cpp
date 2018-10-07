#include "FiringModule.h"

FiringModule::FiringModule(int address, int usable_cues)
: _address(address),
  _usable_cues(usable_cues)
{
  //Set up cues and show object
  _cues = new Cues(usable_cues);
  _show = new Show(_cues);

  //Set up rf communications
  _radio = new CRadio();
  _radio->begin(_address);
}
void FiringModule::handler()
{
  //Call the handlers
  _cues->handler();
  _show->handler();

  //Check for incoming message(s)
  if(_radio->available())
  {
    //Process it
    processMessage(_radio->getMessage());
  }
}
void FiringModule::processMessage(Message msg)
{
  String cmd;
  cmd = splitstr(msg.msg.command, 0);

  //Check if command is valid
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
    testCues(msg);
  }
  else if(cmd == "security")
  {
    //TODO implement an object to manage securities
  }
  else
  {
    send(CODE_COMMAND_NOT_UNDERSTOOD);
  }
}
/*
------------------Commands
 */
void FiringModule::info()
{
  String infoStr = "info ";
  //Add version
  infoStr += String(FM_VERSION);
  infoStr += String(" ");
  //Send it
  Message msg(_address);
  msg.encodeMessage(infoStr, "0");
  send(msg);
}
void FiringModule::selfcheck()
{
  //TODO : check securities here
  send(CODE_OK);
}
void FiringModule::testCues(Message msg)
{
  //TODO trigger relay
  //TODO check securities
  //TODO implement a tester object
  //TODO trigger relay
}
/*
End of commands
 */
void FiringModule::send(int code)
{
  String command = String("e ") + String(code);
  //send to master
  _radio->send(command, 0);
}
void FiringModule::send(Message msg)
{
  _radio->send(&msg);
}
Cues* FiringModule::getCues()
{
  return _cues;
}
String FiringModule::splitstr(String s, int row)
{
  String textToReturn = "";

  //Iterate over characters
  int i = 0;
  //Iterate over words
  int i2 = 0;

  int maxLength = s.length();

  while(i2 != row)
  {
    textToReturn = "";
    while(s[i] != '\0' && s[i] != 32)
      {
        textToReturn += s[i];
        i++;
        if(i > maxLength)
        {
          return String("Command is too long");
        }
      }
      i++;
      //Skip the spaces
      while(s[i] == 32)
        {
          i++;
        }
      i2++;
  }
  return textToReturn;
}
FiringModule::~FiringModule()
{
  delete _radio;
  delete _show;
  delete _cues;
}
