#include "Cues.h"
/**
 * Construct a set of Cues
 * @param set       function that turns on cues eg: digitalWrite
 * @param cues      cues pins assignements as a pointer
 * @param nbOfCues  The nb of elements in 'cues' array
 */
Cues::Cues(void (*set)(int, bool), unsigned int *cues, size_t nbOfCues)
{
  this->_cuePins = cues;
  this->_cues = new long[nbOfCues];

  //Dont know if it works to pass a function like that
  this->_set = set;
}

/**
 * Handles cues sets to LOW
 */
void Cues::handler()
{
  size_t arrSize = sizeof(*this->_cues)/sizeof(long);
  for(size_t i=0; i<=arrSize; i++)
  {
    unsigned long time = millis();
    if((this->_cues[i]) >= time)
    {
      //i + 1 -> to get the 'real' cue
      this->set(i + 1, LOW);
      this->_cues[i] = 0;
    }
  }
}
/**
 * Set any cue a given value
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
    this->_set(this->_cuePins[cue - 1], HIGH);
  }
  else if(value == LOW)
  {
    //Actualy set the cue to LOW
    this->_set(this->_cuePins[cue - 1], LOW);
  }
}
Cues::~Cues()
{
  delete this->_cues;
}
