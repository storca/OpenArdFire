#include "FiringModule.h"

FiringModule::FiringModule(unsigned int &testPin, Logger &log, bool testPinState)
{
  this->_logger = &log;
  this->_testPin = &testPin;

  this->welcome();
  this->_logger->info("This firing system has " + String(sizeof(*this->_cuePins)) + " cues");
  this->_logger->info("Test pin is D" + String(*this->_testPin));
  this->_logger->info("Test pin state must be "+String(this->_testPinState));
}

void FiringModule::welcome()
{
  this->_logger->log("----------------------------------------------", INFO, 0);
  this->_logger->log("OpenArdFire Developpement Version", INFO, 0);
  this->_logger->log("Test sequence : / abcdefghijklmnopqrstwxyz 0123456789", DEBUG, 0);
  this->_logger->log("Developped by EPAILLY Thomas on 23-05-2018", INFO, 0);
  this->_logger->log("This module is able to interpret commands, see doc", INFO, 0);
  this->_logger->log("----------------------------------------------", INFO, 0);

}

/**
 * Set a security of the x securities defined
 * @param securityId Security Id eg. 0, 1, 2...
 * @param state      Status of the security (false -> enabled, true->disabled)
 */
void FiringModule::setSecurity(int securityId, bool *state)
{
  this->_securities[securityId] = *state;
}

/**
 * Checks if the firing module's securities are disabled
 * @return true->securities are disabled
 */
bool FiringModule::isSafe()
{
  for(unsigned int i=0; i<= sizeof(this->_securities); i++)
  {
    //If security is set to false
    if(!this->_securities[i])
    {
      String msg = "Security nbr " + String(i) + " is enabled";
      this->_logger->warn(msg);
      return false;
    }
  }
  return true;
}

/**
 * Test a single cue
 * @param  cue         Cue to test
 * @param  analogLimit Limit to return a true test statement
 * @return             true -> cue is ready
 *                     false -> cue is not connected
 */
bool FiringModule::test(int cue, int analogLimit)
{
  //Enabling test relay
  digitalWrite(*this->_testPin, LOW);

  //Cues.h implementation here

  //Disabling test relays
  digitalWrite(*this->_testPin, HIGH);

  //this->_logger->log("Result of cue "+ cue +" : "+result, DEBUG, true);

  if(result >= analogLimit)
  {
    return true;
  }
  else
  {
    return false;
  }

}

/**
 * Ignite a given cue
 * @param  cue      Cue to ignite
 * @param  duration Ignite duration
 * @return          true if ignited, false if securities are enabled
 */
bool FiringModule::ignite(int cue, int duration)
{
  if(this->isSafe())
  {
      //Cues.h implementation here

      return true;
  }
  else
  {
      return false;
  }

}

FiringModule::~FiringModule()
{

}
