#ifndef FIRINGMODULE_H
#define FIRINGMODULE_H

#define FM_NB_SECURITIES 3

#include <Arduino.h>
#include <Logging.h>

class FiringModule
{
public:
  FiringModule(int &cues, int &testPins, Logger &log);
  ~FiringModule();
private:
  int *_cuePins;
  int *_testPins;
  bool _securities[FM_NB_SECURITIES];
  Logger *_logger;
};

#endif
