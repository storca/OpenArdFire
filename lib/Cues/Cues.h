#ifndef CUES_H
#define CUES_H
/*
This library is the ♥ of satelites, it is this library that
has physical impact on igniters :)

This library handles every single cue wait time without pausing with delay()
It handles only one set of cues (1 mcp)
NOTE : trigger(1) sets the first cue to HIGH for 500ms
NOTE : No way to trigger cues to LOW
NOTE : trigger(0) will not work, this function takes cue _numbers_ (from 1 to any)
 */

#include <Arduino.h>
#include <Adafruit_MCP23017.h>

#ifndef C_MCPS
//2 MCPs on respective addresses 0 and 1
#define C_MCPS {0x20, 0x21}
// Addresses are from 0x20 to 0x27
// MCP address : 0 1 0 0 A2 A1 A0
//http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf p15
#endif

class Cues
{
public:
  Cues(uint8_t usableCues, uint8_t relay_pin);

  void handler();
  void authorise(bool status);
  bool authorised();
  void trigger(uint8_t cue);
  void set(uint8_t pin, bool value);

  void begin_test();
  bool test(uint8_t cue, bool state);
  void end_test();

  const uint8_t* getNumberOfCues();

  ~Cues();

  //How much time the cues are on
  unsigned int _onTime = 500;

private:
  //Status of the security
  //Has to be false to trigger cues
  bool _security = true;
  //Number of cues max 128
  const uint8_t *_nbOfCues;

  //Is test relay enabled ?
  bool _testing = false;
  const int *_test_relay;

  //Stores cues on time
  // For example : when a cue was turned on at a time t,
  // the handler will turn it off at the time t + _onTime
  // This value (t + _onTime) is stored in this array
  // It is a pointer because of the NbOfCues that is variable
  uint32_t *_cues;

  Adafruit_MCP23017 *_mcps;
  size_t _numberOfMcps;

  //Init MCPs
  void init();

  void setMcp(uint8_t, bool value, bool notACue=false);

};

#endif //CUES_H
