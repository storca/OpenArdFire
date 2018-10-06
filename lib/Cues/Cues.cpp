#include "Cues.h"
/**
 * New set of cues on the specified MCPs
 * @param usableCues Number of pins from the MCP that will be used as cues
 */
Cues::Cues(uint8_t usableCues, uint8_t relay_pin)
{
  _nbOfCues = new const uint8_t(usableCues);
  _cues = new uint32_t[*_nbOfCues];
  _test_relay = new const int(relay_pin);
  init();
}


void Cues::init()
{
  //Get the bytes from macro
  byte mcpAddresses[] = C_MCPS;

  //Calculate the number of MCPs
  _numberOfMcps = sizeof(mcpAddresses) / sizeof(byte);
  _mcps = new Adafruit_MCP23017[_numberOfMcps];

  for(size_t i=0; i<_numberOfMcps; i++)
  {
    //Call constructor
    _mcps[i] = Adafruit_MCP23017();
    //Init the mcp with it's I2C address
    _mcps[i].begin(mcpAddresses[i]);
  }
}

/**
 * Handles cues sets to LOW
 */
void Cues::handler()
{
  for(size_t i=0; i<*_nbOfCues; i++)
  {
    if(_cues[i] > millis())
    {
      //i+1 because it's a cue
      setMcp(i+1, LOW);
    }
  }
}
/**
 * Set any cue number greater than _nbOfCues
 * Does not trigger cues, only pins that are not used
 * This takes cue numbers not pins
 * @param cue   Cue number, > _nbOfCues
 * @param value HIGH or LOW (LOW is set automaticly via handler)
 */
void Cues::set(uint8_t cue, bool value)
{
  if(cue > *_nbOfCues)
  {
    setMcp(cue, value, true);
  }
}
/**
 * Set the state of a cue on the MCP
 * @param cue   Cue to set
 * @param value HIGH or LOW
 */
void Cues::setMcp(uint8_t cue, bool value, bool notACue)
{
  //Do not trigger a cue that is higher than the usable cues
  //Trigger otherwise if notACue is set to true
  if(cue > *_nbOfCues && !notACue)
  {
    return;
  }

  uint8_t mcpToTrigger = 1;
  while(mcpToTrigger*16 < cue)
  {
    mcpToTrigger++;
  }
  //Decrement one to use this to select MCP
  //Because if cue <= 16, will return 1,
  //but the actual mcp to trigger is _mcps[0]
  mcpToTrigger--;

  //Little formula here that returns the pin to trigger on MCP
  //NOTE : returns from 1 to 16
  byte pinToTrigger = (16 - ((mcpToTrigger * 16) - cue));

  //NOTE : incomprehension here ? why a '.' and not a '->'
  //       coz _mcps is a pointer
  _mcps[mcpToTrigger].digitalWrite(pinToTrigger, value);

}
/**
 * Triggers a cue number (from 1 to any)
 * @param cue Cue number to trigger
 */
void Cues::trigger(uint8_t cue)
{
  //Do not trigger a cue that is higher than the usable cues
  //Do not trigger if security is enabled
  if(cue > *_nbOfCues || _security)
  {
    return;
  }

  //Turn the cue on
  setMcp(cue, HIGH);
  //Add it to the array
  //There is a -1 at the begining because cues begins from 1 to any and not 0
  //There is -1 at the end because of the operator '<' in handler for optimisation
  _cues[cue-1] = millis() + _onTime - 1;
}
/**
 * Get the number of cues
 * @return The number of cues
 */
const uint8_t* Cues::getNumberOfCues()
{
  return _nbOfCues;
}
/**
 * Authorise cue triggering
 * @param status autorise = true
 */
void Cues::authorise(bool status)
{
  //If status == true, security is disabled
  _security = !status;
}
/**
 * Checks if cue triggering is possible
 * @return true if possible
 */
bool Cues::authorised()
{
  return !_security;
}
/**
 * @brief Begin cue testing
 * Triggers test relay and ensure the security is enabled
 * 
 */
void Cues::begin_test()
{
  //Enable relay
  digitalWrite(*_test_relay, LOW);
  //Enable testing
  _testing = true;
  //Enable security
  //Avoid triggering cues while testing
  _security = true;
}
/**
 * @brief Test a cue if testing enabled
 * _Sets any cue to the state given, ensures the system is secured_
 * 
 * @param cue Cue to test
 * @param state State to set on the cue (HIGH/LOW)
 * @return true State set succefull
 * @return false Unable to set state of the cue
 */
bool Cues::test(uint8_t cue, bool state)
{
  if(_testing)
  {
    setMcp(cue, state);
    return true;
  }
  else
  {
    return false;
  }
}
/**
 * @brief End cue test
 * Enables security
 * 
 */
void Cues::end_test()
{ 
  //Make sure all cues are set to LOW
  for(size_t i=0; i<*_nbOfCues; i++)
  {
    setMcp(i+1, LOW);
  }
  //Keep security enabled
  _security = true;
  //Disable testing
  _testing = false;
  //Disable relay
  digitalWrite(*_test_relay, HIGH);

}
Cues::~Cues()
{
  delete this->_cues;
  delete _mcps;
  delete _test_relay;
}