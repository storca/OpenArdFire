#ifndef CTIME_H
#define CTIME_H

#include <Wire.h>
//Add to platformio

#define CONFIG_UNIXTIME
#include <ds3231.h>

class CTime
{
public:
  CTime();
  void update(unsigned long unix_time);
  unsigned long get();
  bool match(unsigned long unix_time);
  ~CTime();
private:
  struct ts _time;
};


#endif
