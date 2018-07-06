#ifndef FIRINGMODULE_H
#define FIRINGMODULE_H

/*
This library manages securities, cues and coordinates communication with actions
  It's the ♥ of our firing system :)
  It can be run in two modes : (not done yet, looking into re-organising library)
      - With RemoteBoxes : it works with remote boxes
      - With ArduinoCues : it works with arduino cues only (for simple systems)
 */

//Number of securities used by the firing module
#define FM_NB_SECURITIES 3

//What is the pin that triggers the relays
#define FM_TESTPIN 6


#include <Arduino.h>
#include <Logging.h>

class FiringModule
{
public:
  FiringModule(unsigned int &testPins, Logger &log, bool testPinState=LOW);
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
  unsigned int *_testPin;
  unsigned int *_analogTestPin;
  //State to set to enable test relays
  bool _testPinState = LOW;
  bool _securities[FM_NB_SECURITIES];
  Logger *_logger;

  void welcome();
};

#endif
