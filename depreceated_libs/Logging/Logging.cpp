#include "Logging.h"

/**
 * Create a Logger object
 * @param s        Local arduino serial port
 * @param rf       LORA serial port
 * @param logLevel level of logging
 */
Logger::Logger(HardwareSerial &s, SoftwareSerial &rf, int logLevel)
{
  this->_logLevel = &logLevel;
  this->_s = &s;
  this->_rf = &rf;
}

/**
 * Send a debug message
 * @param msg message to send
 */
void Logger::debug(String &msg)
{
  if(*(this->_logLevel) > 2)
  {
    this->print(msg, 3);
  }
}

/**
 * Send a info message
 * @param msg message to send
 */
void Logger::info(String &msg)
{
  if(*(this->_logLevel) > 1)
  {
    this->print(msg, 2);
  }
}

/**
 * Send a warning message
 * @param msg message to send
 */
void Logger::warn(String &msg)
{
  if(*(this->_logLevel) > 0)
  {
    this->print(msg, 1);
  }
}

/**
 * Send an error message
 * @param msg message to send
 */
void Logger::error(String &msg)
{
  if(*(this->_logLevel) > -1)
  {
    this->print(msg, 0);
  }
}

void Logger::log(const char *msg, int logLevel, bool logMessageType)
{
  this->print(msg, logLevel, logMessageType);
}

/**
 * Print a message to the two serial ports
 * @param msg   message to send
 * @param level level of the message (debug->3, info->2 etc)
 */
void Logger::print(String &msg, int level, bool logMessageType)
{
  this->send(&msg, level, logMessageType);
}
void Logger::print(String *msg, int level, bool logMessageType)
{
  this->send(msg, level, logMessageType);
}
void Logger::print(const char *msg, int level, bool logMessageType)
{
  String str = String(msg);
  this->send(&str, level, logMessageType);
}

void Logger::send(String *msg, int level, bool logMessageType)
{
  String buffer = "";
  if(LOG_LOG_MESSAGE_TYPE && logMessageType)
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
    buffer += *msg;

    //Print / Send the message
    this->_s->println(buffer);
    this->_rf->println(buffer);
  }
  else
  {
    //Print / Send the message without the type of message
    this->_s->println(buffer);
    this->_rf->println(buffer);
  }
}

/**
 * Send an error code
 * @param code Error code to send
 */
void Logger::code(int &code)
{
  String strCode = String(code);
  this->print(strCode, 0, false);
}
