#include <Arduino.h>
#include <Cues.h>
#include <FiringModule.h>
#include <ErrorCodes.h>

class Tester
{
public:
  Tester(FiringModule *parent, uint8_t testPin);
  bool test_all();
  bool test(uint8_t cue);
  void handler();
  bool get_test_status();
  bool get_result(uint8_t cue);
  bool* get_results();
  ~Tester();

  //How long does the measurement last
  int measurement_duration = 300;

private:
  //Test type
  enum {Single, Multiple};

  FiringModule *_parent;
  Cues *_cues;
  const int *_test_pin;
  int _current_cue;
  bool _test_running = false;
  bool _test_type;
  unsigned long next_measurement;
  bool *_measurements;
};
