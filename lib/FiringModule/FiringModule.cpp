#include "FiringModule.h"

/**
 * @brief Construct a new Firing Module:: Firing Module object
 * 
 * @param address Address of the firing module
 * 
 * Which address the firing module has on the subnetwork
 * @param usable_cues How much cues do we use
 * @param testRelay The test relay we use
 */
FiringModule::FiringModule(int address, int usable_cues, int test_pin, Cues::Relay *testRelay)
: _address(address),
  _usable_cues(usable_cues),
  _test_pin(test_pin),
  _test_relay(testRelay)
{
  //Set up cues and show object
  _cues = new Cues(_usable_cues, _test_relay);
  _show = new Show(_cues);
  _tester = new Tester(this, _test_pin);

  //Set up rf communications
  _radio = new CRadio(_address);
  _radio->begin();
  _mymsg = new Message(_address);
}
/**
 * @brief Handles things
 * 
 * Has to be called in loop()
 * 
 */
void FiringModule::handler()
{
  //Call the handlers
  _cues->handler();
  _show->handler();
  _tester->handler();

  //Check for incoming message(s)
  if(_radio->available())
  {
    //Process it
    processMessage(_radio->getMessage());
  }
}
/**
 * @brief Process incoming message
 * 
 * @param msg Message to process
 */
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


/**
 * @brief Send info about the system
 * 
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
/**
 * @brief Performs selfchecks
 * 
 * _Not used for now, may be usefull in the future_
 * 
 */
void FiringModule::selfcheck()
{
  //TODO : check securities here
  send(CODE_OK);
}
/**
 * @brief Test cues with the message sent
 * 
 * @param msg Details about what cue(s) to test
 */
void FiringModule::testCues(Message msg)
{
  //Message check
  msg.getMessage();
  //Set relay to test
  digitalWrite(_test_relay->pin, _test_relay->state);
  
  

  //Set relay to firing mode
  digitalWrite(_test_relay->pin, !_test_relay->state);
}
/*
End of commands
 */

/**
 * @brief Send error code to master
 * 
 * @param code Error code to send
 * 
 * Error codes are defined in ErrorCodes.h<br>
 * To add one, please use the python script located in 
 * /helpers/errorcodes/errocode.py
 */
void FiringModule::send(int code)
{
  String command = String("e ") + String(code);
  //send to master
  _radio->send(command, 0);
}
/**
 * @brief Send any message to any device
 * 
 * @param msg 
 */
void FiringModule::send(Message msg)
{
  _radio->send(&msg);
}
/**
 * @brief Get private _cues object
 * 
 * @return Cues* 
 */
Cues* FiringModule::getCues()
{
  return _cues;
}
/**
 * @brief Split a string
 * 
 * @param s String to split
 * @param row Part of the array to return
 * @return String Splitted string s[row]
 */
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
/**
 * @brief Destroy the Firing Module:: Firing Module object
 * 
 */
FiringModule::~FiringModule()
{
  delete _radio;
  delete _show;
  delete _cues;
  delete _tester;
}
