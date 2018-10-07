//Tester.cpp
/**
 * @file Tester.cpp
 * @author Storca (storca@mail.com)
 * @brief Tester source
 * @version 0.1
 * @date 2018-10-07
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "Tester.h"

/**
 * @brief Construct a new Tester:: Tester object
 * 
 * @param parent 
 * @param testpin Where is attached the test output
 */
Tester::Tester(FiringModule *parent, uint8_t testpin)
{
  //Set parent
  _parent = parent;
  //Retreive cues
  _cues = _parent->getCues();
  //Set test pin
  _test_pin = new const int(testpin);
  //Create a array for measurements results
  _measurements = new bool[_parent->_usable_cues];
  //Set pin as input pullup
  //DOUBLE CHECK: if it's a pullup and not a pulldown
  pinMode(testpin, INPUT_PULLUP);
}
/**
 * @brief Test all the cues
 * 
 */
bool Tester::test_all()
{
  //If show is running, send error
  if(_parent->_show->running())
  {
    _parent->send(CODE_UNABLE_TO_TEST);
  }
  else if(_parent->test_authorised)
  {
    //Set the test to running
    _test_running = true;
    //We test multiple cues here
    _test_type = Multiple;
    //Set the current cue to 0
    _current_cue = 0;
    //Unauthorise the test
    _parent->test_authorised = false;

    //Begin test
    _cues->begin_test();
    next_measurement = millis() + measurement_duration;
    _cues->test(_current_cue+1, HIGH);
  }
}
/**
 * @brief Test a single cue
 * 
 * @param cue Cue to test
 * @return true if test is running
 * @return false if test is not able to run
 */
bool Tester::test(uint8_t cue)
{
  if(_test_running)
  {
    //TODO: maybe use errorcodes here?
    return false;
  }
  else if(!_parent->test_authorised)
  {
    return false;
  }
  else
  {
    //Set the test to running
    _test_running = true;
    //Test is only for a single cue
    _test_type = Single;
    //Set the cue to test
    _current_cue = cue-1;
    //Unauthorise test
    _parent->test_authorised = false;

    //Begin test
    _cues->begin_test();
    next_measurement = millis() + measurement_duration;
    _cues->test(_current_cue+1, HIGH);
  }
}
/**
 * @brief Handles tests when needed
 * 
 */
void Tester::handler()
{
  if(_test_running)
  {
    switch(_test_type)
    {
      case Multiple:
        if(millis() > next_measurement)
          {
            if(_current_cue < _parent->_usable_cues)
            {
              //Read value
              _measurements[_current_cue] = digitalRead(*_test_pin);
              //Set the cue to LOW
              _cues->test(_current_cue+1, LOW);
              //Set next cue
              _current_cue++;
              next_measurement = millis() + measurement_duration;
              _cues->test(_current_cue+1, HIGH);
            }
            else
            {
              //Stop the test
              _cues->end_test();
              _test_running = false;
            }
          }
      break;
      case Single:
        if(millis() > next_measurement)
        {
          //Measure
          _measurements[_current_cue] = digitalRead(*_test_pin);
          _cues->test(_current_cue+1, LOW);

          //Stop the test
          _cues->end_test();
          _test_running = false;
        }
      break;
    }
  }
}
/**
 * @brief Returns whether the test is running
 * 
 * @return true Test is running
 * @return false Test is not running
 */
bool Tester::get_test_status()
{
  return _test_running;
}
/**
 * @brief Checks if a cue is operational
 * _If the current flows through the igniter and back to the system_
 * 
 * @param cue Cue to monitor
 * @return true Cue is operational
 * @return false Cue is cutted / not connected
 */
bool Tester::get_result(uint8_t cue)
{
  if(cue < _parent->_usable_cues+1)
  {
    return _measurements[cue-1];
  }
}
/**
 * @brief Returns all the results of the measurements
 * _If the current flows through the igniter and back to the system_
 * 
 * @return true Cue is operational 
 * @return false Cue is cutted / not connected
 */
bool* Tester::get_results()
{
  return _measurements;
}
Tester::~Tester()
{
  delete _measurements;
  delete _test_pin;
}