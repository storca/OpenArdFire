#ifndef CUES_H
#define CUES_H
/**
 * @file Cues.h
 * @author Storca (storca@mail.com)
 * @brief Handles cue triggering without using the delay() function.
 * So the cue triggering is more precise.
 * @version 0.1
 * @date 2018-10-07
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <Arduino.h>
#include <Adafruit_MCP23017.h>

#ifndef C_MCPS
/**
 * @brief This is where the MCPs addresses should be putted.<br>
 * 
 * Addresses are from 0x20 to 0x27.<br>
 * Binary MCP address : (0 1 0 0 A2 A1 A0).<br>
 * http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf page 15
 * 
 */
#define C_MCPS {0x20, 0x21}

#endif
/**
 * @brief This class is the ♥ of satelites, it is this library that
 * has physical impact on igniters :)
 * 
 * This library handles every single cue wait time without pausing with delay()<Br>
 * It can handles multiple MCPs via the C_MCPS marco<br>
 * NOTE : trigger(1) sets the first cue to HIGH for 500ms<br>
 * NOTE : No way to trigger cues to LOW<br>
 * NOTE : trigger(0) will not work, this function takes cue _numbers_ (from 1 to any)
 * 
 */
class Cues
{
public:

  /**
   * @brief Used to store information about a relay
   * 
   */
  struct Relay
  {
    /**
     * @brief The pin on which the relay is attached to
     * 
     * eg : 9
     */
    unsigned const int pin;
    /**
     * @brief The state when the relay is closed
     * 
     * eg : LOW
     * 
     */
    const bool state;
  };

  Cues(uint8_t usableCues, Relay *test_relay);

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

  /**
   * @brief How much time cues are set to on when calling trigger() function
   * 
   */
  unsigned int _onTime = 500;

private:
  /**
   * @brief Status of the security
   * 
   * Has to be false to trigger cues
   * 
   */
  bool _security = true;
  
  /**
   * @brief How much cues do we handle
   * 
   */
  const uint8_t *_nbOfCues;

  /**
   * @brief Tells if the system is in the "testing" state
   * 
   * When it's testing (when begin_test() was called), 
   * no cues can be triggered via trigger() function.<br>
   * Instead, test relays are set in 'test' position and cues states
   * should be set via the test() function
   * 
   */
  bool _testing = false;
  /**
   * @brief What test relay to trigger
   * 
   */
  Relay *_test_relay;

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
