/**
 * @file Tester.h
 * @author Storca (storca@mail.com)
 * @brief A class that handles single or multiple cue testing
 * @version 0.1
 * @date 2018-10-07
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef TESTER_H
#define TESTER_H

#include <Arduino.h>
#include <Cues.h>
#include <ErrorCodes.h>

/**
 * @brief This class is used to test the firing module's cues.
 * 
 * It can test single or multiple cues one by one
 * 
 */

/*
Depreceated just for reference lol
//Forward declaration of FiringModule
//https://stackoverflow.com/questions/1429336/cross-referencing-included-headers-in-c-program
class FiringModule;
*/

class Tester
{
public:
  Tester(Cues *cues, const int testPin);
  bool test_all();
  bool test(uint8_t cue);
  void handler();
  bool get_test_status();
  void allow_test(bool test_allowed);
  bool get_result(uint8_t cue);
  bool* get_results();
  ~Tester();

  /**
   * @brief How long does the measurement last in milliseconds
   * 
   */
  int measurement_duration = 300;

private:
  enum {Single, Multiple};

  Cues *_cues;
  const int *_test_pin;
  int _current_cue;
  bool _test_running = false;
  bool _test_allowed = false;
  bool _test_type;
  unsigned long next_measurement;
  bool *_measurements;
};

#endif