#ifndef RFID_H
#define RFID_H

/*
This library handles RFID identification for FiringModule
tags variable has to be an int[x][14]
 */

#include <Arduino.h>
#include <Logging.h>

class RFID
{
public:
  RFID(HardwareSerial *rfid, long baudrate);
  RFID(SoftwareSerial *rfid, long baudrate);

  void addTags(int tags[][14]);

  void handler();

  ~RFID();
private:
  HardwareSerial *_rfidh;
  SoftwareSerial *_rfids;

  int _tags[][14];
};

#endif
