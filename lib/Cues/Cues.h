#ifndef CUES_H
#define CUES_H

/*
This library handles every single cue wait time without pausing with delay()
 */

#include <Arduino.h>

class Cues
{
public:
  Cues(void (*set)(int, bool), unsigned int *cues, size_t nbOfCues);

  void handler();
  void set(int cue, bool value);

  ~Cues();


private:

  //Cue pins
  unsigned int *_cuePins;

  //This variable keeps the time when the cue is supposed to set itself to low
  unsigned long *_cues;

  //How much time the cues are on
  unsigned int _onTime = 500;

  void (*_set)(int, bool);
};

#endif //CUES_H