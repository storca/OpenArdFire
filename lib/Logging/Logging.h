/*
    NOTE: This library is very useless in my case because strings take too much
    memory, so they've been replaced by error codes
    I just leave it here in case anyone needs it :)
 */

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

//Log level types
#define ERR 0
#define WARN 1
#define INFO 2
#define DEBUG 3

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

  void log(const char *msg, int logLevel, bool logMessageType=LOG_LOG_MESSAGE_TYPE);

  void code(int &code);

private:
  void print(String &msg, int level, bool logMessageType=LOG_LOG_MESSAGE_TYPE);
  void print(String *msg, int level, bool logMessageType=LOG_LOG_MESSAGE_TYPE);
  void print(const char *msg, int level, bool logMessageType=LOG_LOG_MESSAGE_TYPE);

  void send(String *msg, int level, bool logMessageType);

  HardwareSerial* _s;
  SoftwareSerial* _rf;
  int* _logLevel;
};

#endif
