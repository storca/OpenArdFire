/**
 * Logging.h ; A library that handles logging on arduino with two
 * serial ports
 * Created by Storca on 23-05-2018
 * LOGLEVEL
 *  -1  -> No Logging
 *   0  -> Errors
 *   1  -> Warnings
 *   2  -> Info
 *   3  -> Debug
 */

#ifndef LOGGING_H
#define LOGGING_H

//Default level of logging
#define LOG_LOGLEVEL 0
//Log message type ? -> Adds a "WARN: " or "ERR: " at the begining of the message
#define LOG_LOG_MESSAGE_TYPE false

#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 * Logger object
 * @param s        Default serial of Arduino
 * @param rf       Second serial such as LORA
 * @param logLevel Level of logging
 */
class Logger{

public:
  Logger(HardwareSerial &s, SoftwareSerial &rf, int logLevel=LOG_LOGLEVEL);
 
  void debug(String &msg);
  void info(String &msg);
  void warn(String &msg);
  void error(String &msg);

private:
  void print(String &msg, int level);

  HardwareSerial* _s;
  SoftwareSerial* _rf;
  int* _logLevel;
};

#endif
