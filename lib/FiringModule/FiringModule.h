#ifndef FIRINGMODULE_H
#define FIRINGMODULE_H

//Number of securities used by the firing module
#define FM_NB_SECURITIES 3

#include <Arduino.h>
#include <Logging.h>

class FiringModule
{
public:
  FiringModule(unsigned int &cues, unsigned int &testPins, Logger &log, bool testPinState=LOW);
  ~FiringModule();

  //Securities
  void setSecurity(int securityId, bool *state);
  bool isSafe();

  //Test
  bool test(int cue, int analogLimit=400);
  //void testSequence(int *cues, int analogLimit=400); to do

  //Ignite
  bool ignite(int cue, int duration=500);

private:
  unsigned int *_cuePins;
  unsigned int *_testPin;
  unsigned int *_analogTestPin;
  //State to set to enable test relays
  bool _testPinState = LOW;
  bool _securities[FM_NB_SECURITIES];
  Logger *_logger;

  void welcome();
};

#endif
