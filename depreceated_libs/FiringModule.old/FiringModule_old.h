#ifndef FIRINGMODULE_H
#define FIRINGMODULE_H

#ifndef FM_VERSION
#define FM_VERSION "dev"
#endif

/*
This library is the brain of satelites, it is this library that
coordinates actions :D
 */

//Number of securities used by the firing module
#define FM_NB_SECURITIES 3

//What is the pin that triggers the relays
#define FM_TESTPIN 6

#include <Arduino.h>
#include <Logging.h>
#include <CommunicationHandler.h>
#include <Radio.h>
#include "ErrorCodes.h"

class FiringModule
{
public:
  FiringModule(uint8_t address);
  void handler();
  ~FiringModule();

  //Securities
  void setSecurity(int securityId, bool state);
  bool isSafe();

  //Test
  bool test(int cue, int analogLimit=400);
  //void testSequence(int *cues, int analogLimit=400); to do

  //Ignite
  bool ignite(int cue, int duration=500);

private:

  //Commands
  void info();
  void selfcheck();
  void testCues(Message msg);

  void sendErrorCode(int errorcode);

  void processMessage(Message msg);

  String splitCommand(String command, int row);

  CRadio *_radio;
  CommunicationHandler *_ch;
  Message *_mymsg;
  unsigned int *_testPin;
  unsigned int *_analogTestPin;
  //State to set to enable test relays
  bool _testPinState = LOW;
  bool _securities[FM_NB_SECURITIES];
};

#endif
