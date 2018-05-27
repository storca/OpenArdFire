#include "FiringModule.h"

FiringModule::FiringModule(int &cues, int &testPins, Logger &log)
{
  this->_cuePins = &cues;
  this->_logger = &log;
  this->_testPins = &testPins;

  this->_logger->info("This firing system has " + String(sizeof(*this->_cuePins)) + " cues");
  this->_logger->info("Test pins are ");
}

FiringModule::~FiringModule()
{

}
