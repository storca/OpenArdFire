#include "Cues.h"
/**
 * New set of cues on the specified MCPs
 * @param usableCues Number of pins from the MCP that will be used as cues
 */
Cues::Cues(uint8_t usableCues)
{
  _nbOfCues = new const uint8_t(usableCues);
  _cues = new uint32_t[*_nbOfCues];
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
      setMcp(i, LOW);
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
  //Trigger othewise if notACue is set to true
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
  //but the actual mpc to trigger is _mcps[0]
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
  if(cue > *_nbOfCues || !_security)
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
Cues::~Cues()
{
  delete this->_cues;
  delete _mcps;
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
