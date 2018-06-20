#ifndef RFID_H
#define RFID_H

/*
This library handles RFID identification for FiringModule
tags variable has to be an int[x][14]
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <RDM6300.h>
#include <Logging.h>

#define RFID_MAX_TAGS 3

#define RFID_SOFTWARE_SERIAL 0
#define RFID_HARDWARE_SERIAL 1

//Serial template
template <typename SerialT>

class RFID
{
public:
  RFID(HardwareSerial *rfid);
  RFID(SoftwareSerial *rfid);

  void addTags(int *tags[14]);
  void addTags(int tags[][14]);

  void handler();

  ~RFID();

private:
  RDM6300<SerialT> *_rfid;

  int _numberOfTags = 0;

  bool _rfidState = false;

  long *_tags;
};

#endif
