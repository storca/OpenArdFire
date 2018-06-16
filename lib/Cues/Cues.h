#ifndef CUES_H
#define CUES_H

/*
This library handles every single cue wait time without pausing with delay()
 */

#include <Arduino.h>
#include <Adafruit_MCP23017.h>

class Cues
{
public:
  Cues(Adafruit_MCP23017 *mcp, unsigned int *cues, size_t nbOfCues);

  void handler();

  ~Cues();


private:

  //Cue pins
  unsigned int *_cuePins;

  //This variable keeps the time when the cue is supposed to set itself to low
  unsigned long *_cues;

  //How much time the cues are on
  unsigned int _onTime = 500;

  Adafruit_MCP23017 *_mcp;

  void set(int cue, bool value);

};

#endif //CUES_H
