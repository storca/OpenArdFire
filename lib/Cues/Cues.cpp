#include "Cues.h"
/**
 * Construct a set of Cues
 * @param set       function that turns on cues eg: digitalWrite
 * @param cues      cues pins assignements as a pointer
 * @param nbOfCues  The nb of elements in 'cues' array
 */
Cues::Cues(Adafruit_MCP23017 *mcp, unsigned int *cues, size_t nbOfCues)
{
  this->_cuePins = cues;

}

/**
 * Handles cues sets to LOW
 */
void Cues::handler()
{
  //TODO : This is a duplicate of Show.h
  //TODO : Need fix
  size_t arrSize = sizeof(*this->_cues)/sizeof(long);
  for(size_t i=0; i<=arrSize; i++)
  {
    unsigned long time = millis();
    if((this->_cues[i]) >= time)
    {
      //i + 1 -> to get the 'real' cue number > 0
      this->set(i + 1, LOW);
      this->_cues[i] = 0;
    }
  }
}
/**
 * Set any cue number a given value eg cue nb 21
 * This takes cue numbers not pins
 * @param cue   Cue number, > 0
 * @param value HIGH or LOW (LOW is set automaticly via handler)
 */
void Cues::set(int cue, bool value)
{
  if(value == HIGH)
  {
    //Save the time when the cue should be set to LOW
    this->_cues[cue - 1] = millis() + this->_onTime;

    //Actualy set the cue to HIGH
    this->_mcp->digitalWrite(this->_cuePins[cue - 1], HIGH);
  }
  else if(value == LOW)
  {
    //Actualy set the cue to LOW
    this->_mcp->digitalWrite(this->_cuePins[cue - 1], LOW);
  }
}
Cues::~Cues()
{
  delete this->_cues;
}
