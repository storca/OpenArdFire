#include "Logging.h"

Logger::Logger(int &logLevel, HardwareSerial &s, SoftwareSerial &rf)
{
  this->_logLevel = logLevel;
  this->_s = s;
  this->_rf = rf;
}
void Logger::debug(String &msg)
{
  if(this->_logLevel > 2)
  {
    Logger::print(msg, 3);
  }
}
void Logger::info(String &msg)
{
  if(this->_logLevel > 1)
  {
    Logger::print(msg, 2);
  }
}
void Logger::warn(String &msg)
{
  if(this->_logLevel > 0)
  {
    Logger::print(msg, 1);
  }
}
void Logger::error(String &msg)
{
  if(this->_logLevel > -1)
  {
    Logger::print(msg, 0);
  }
}
void Logger::print(String &msg, int &level)
{
  String buffer = "";
  if(LOG_LOG_MESSAGE_TYPE)
  {
    switch (level)
    {
      case 0: //Error
        buffer += "ERR: ";
      break;
      case 1://Warning
        buffer += "WARN: ";
      break;
      case 2:
        buffer += "INFO: ";
      break;
      case 3:
        buffer += "DEBUG: ";
      break;
      default:
        buffer += "UKN: ";
      break;
    }
    //Add the message to the buffer
    buffer += msg;

    //Print / Send the message
    this->_s.println(buffer);
    this->_rf.println(buffer);
  }
  else
  {
    //Print / Send the message without the type of message
    this->_s.println(buffer);
    this->_rf.println(buffer);
  }
}
