#include "Show.h"
/**
 * New show
 * @param cues set of cues
 */
Show::Show(Cues *cues)
:_triggeredCues(0)
{
  _cues = cues;
  _nbOfCues = _cues->getNumberOfCues();
  _triggerTime = new unsigned long[*_nbOfCues];
}
/**
 * Set a single cue properties
 * @param  cue         Cue to set
 * @param  triggerTime The time after the beginning of the show when the cue have to be triggered (ms)
 * @return             true if success
 */
bool Show::set(uint8_t cue, unsigned long triggerTime)
{
  if(cue > *_nbOfCues)
  {
    return false;
  }
  //
  _triggerTime[cue - 1] = triggerTime;
}
/**
 * Save a show in the specified eeprom address
 * @param  address Address where the show is stored
 * @return         true if success
 */
bool Show::save(int address)
{
  //TODO : Test if it works
  if(EEPROM.isReady())
  {
      EEPROM.writeLong(address, *_triggerTime);
      return true;
  }
  return false;
}
/**
 * Import a show from an address
 * @param  address Address where the show was stored
 * @return         true if success
 */
bool Show::import(int address)
{
  if(EEPROM.isReady())
  {
    *_triggerTime = EEPROM.readLong(address);
    return true;
  }
  return false;
}
/**
 * @brief Allow or prevent the show from starting
 * 
 * @param is_show_allowed is the show allowed
 */
void Show::allow_show(bool is_show_allowed)
{
  _is_show_allowed = is_show_allowed;
}
/**
 * Start a show with parameters given
 */
int Show::start()
{
  //Is cue triggering possible ?
  if(!_cues->authorised())
  {
    return 2;
  }
  else if(!_is_show_allowed)
  {
    return 3;
  }
  {
    _triggeredCues = 0;
    _startTime = millis();
    _running = true;

    return true;
  }
}
/**
 * Check if the show is running
 * @return true if running
 */
bool Show::is_show_running()
{
  return _running;
}
/**
 * Handles the shows :D
 */
void Show::handler()
{
  if(_running)
  {
    for (size_t i = 0; i < *_nbOfCues; i++)
    {
      if(_startTime + _triggerTime[i] > millis())
      {
        //Avoid triggering the cues multiple times
        _triggerTime = 0;

        //Triger the cue :D
        _cues->trigger(i + 1);

        //Say that we triggered a cue
        _triggeredCues++;
      }
    }
    if(_triggeredCues >= *_nbOfCues)
    {
      //Stop the show
      _running = false;
    }
  }
}
Show::~Show()
{
  delete _triggerTime;
}
