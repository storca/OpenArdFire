#ifndef CTIME_H
#define CTIME_H

#include <Wire.h>
#include <Time.h>

//Add to platformio
#include <DS3231.h>


//TODO : calculate offset for time transmission since we'd like
//to be precise in milliseconds
//UPDATE : time offset will be managed by PC software
class CTime
{
public:
  CTime();
  void update(unsigned long unix_time);
  unsigned long get();
  bool match(unsigned long unix_time);
  void handler();
  ~CTime();
private:
  unsigned long toUnixTime(DateTime t);
  void setTime(String timeStr);
  String cutCmd(String cmdInput, int argumentRow);

  DateTime _time;
  //If update message with offset has been received
  bool _needUpdate = false;
  //milliseconds offset
  int _offset = 0;
  DS3231 *_rtc;
};


#endif
