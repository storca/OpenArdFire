#ifndef SHOW_H
#define SHOW_H

#include <EEPROMEx.h>

class Show
{
public:
  Show(size_t nbrOfCues);
  void setStartTime(unsigned long startTime);
  ~Show();
  struct Cue
  {
    //Highest cue -> 255 (256)
    byte cue;
    unsigned long time;
  };
private:
  Cue *_show = nullptr;

};


#endif
