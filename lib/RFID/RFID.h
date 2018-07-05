#ifndef RFID_H
#define RFID_H

/*
This library handles RFID identification for FiringModule
tags variable has to be an unsigned long long (or unsigned int_64)
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <RDM6300.h>
#include <Logging.h>
#include <FiringModule.h>

#define RFID_MAX_TAGS 3


class RFID
{
public:
  RFID(SoftwareSerial *rfid, FiringModule *fm);

  void addTags(unsigned long long *tags);
  void addTag(unsigned long long *tag);

  bool valid();

  void handler();

  ~RFID();

private:
  size_t _numberOfTags = 0;

  RDM6300<SoftwareSerial> *_rfid;
  FiringModule *_fm;

  bool _rfidState = false;

  unsigned long long *_tags;
};

#endif
