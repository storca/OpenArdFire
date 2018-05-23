#include "Logging.h"

/**
 * Create a Logger object
 * @param s        Local arduino serial port
 * @param rf       LORA serial port
 * @param logLevel level of logging
 */
Logger::Logger(HardwareSerial &s, SoftwareSerial &rf, int logLevel=LOG_LOGLEVEL)
{
  this->_logLevel = logLevel;
  this->_s = &s;
  this->_rf = &rf;
}

/**
 * Send a debug message
 * @param msg message to send
 */
void Logger::debug(String &msg)
{
  if(this->_logLevel > 2)
  {
    print(msg, 3);
  }
}

/**
 * Send a info message
 * @param msg message to send
 */
void Logger::info(String &msg)
{
  if(this->_logLevel > 1)
  {
    print(msg, 2);
  }
}

/**
 * Send a warning message
 * @param msg message to send
 */
void Logger::warn(String &msg)
{
  if(this->_logLevel > 0)
  {
    print(msg, 1);
  }
}

/**
 * Send an error message
 * @param msg message to send
 */
void Logger::error(String &msg)
{
  if(this->_logLevel > -1)
  {
    this->print(msg, 0);
  }
}

/**
 * Print a message to the two serial ports
 * @param msg   message to send
 * @param level level of the message (debug->3, info->2 etc)
 */
void Logger::print(String &msg, int level)
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
