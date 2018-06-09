#ifndef REMOTEBOX_H
#define REMOTEBOX_H

/*
This library is managning the "remote boxes" connected to any receiver
Each receiver can support up to 8 remote boxes with 15 cues per box (one cue
dedicated to test relays / MOSFETS)
Why only 8 remote boxes ?
Because the MCP23017 is addresed on 3 bits -> 2³ = 8
  Fix for this limitation => Use I2C address changers
                          => Use another I2C line
Each box has an I2C input and has a specific I2C address
Every box is connected on the same I2C cable
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>

class RemoteBox
{
public:
  RemoteBox(unsigned int address, unsigned int firstCue, unsigned int lastCue);
  void begin();
  ~RemoteBox();
private:
  unsigned int _i2cAddress;
  unsigned int _cuesRange[2];
  TwoWire *_Wire;

  Adafruit_MCP23017 mcp;

};


#endif
