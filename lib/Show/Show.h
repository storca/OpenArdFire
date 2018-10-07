#ifndef SHOW_H
#define SHOW_H

#include <EEPROMex.h>
#include <Cues.h>

/*
This library handles cue trigger on an automatic show
Idea of the memory usage of a show :
 ~4 bytes / cue
 */

class Show
{
public:
  Show(Cues *cues);
  //void setStartTime(unsigned long startTime);

  void start();
  bool running();
  bool set(uint8_t cue, unsigned long triggerTime);
  bool save(int address);
  bool import(int address);
  void handler();
  ~Show();

private:
  //Cues pointer
  Cues *_cues = nullptr;
  //Trigger time for each cue
  unsigned long *_triggerTime;
  //Number of triggered cues
  uint8_t _triggeredCues;
  //Number of cues
  const uint8_t *_nbOfCues;
  //Start time
  unsigned long _startTime;
  //Show is running ?
  bool _running = false;
};


#endif
