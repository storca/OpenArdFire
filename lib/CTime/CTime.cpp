#include "CTime.h"

CTime::CTime()
{
  _rtc = new DS3231();
  //Use 24h format
  _rtc->setClockMode(false);
  Wire.begin();
}
/**
 * Get the actual unix time
 * @return Unix time
 */
unsigned long CTime::get()
{
  bool century;
  bool h12;
  bool pm;
  //More code...
  DateTime *currentTime = new DateTime(
    _rtc->getYear(), _rtc->getMonth(century), _rtc->getDate(),
    _rtc->getHour(h12, pm), _rtc->getMinute(),
    _rtc->getSecond()
  );
  _time = *currentTime;
  return _time.unixtime();
}
/**
 * Checks if the given time is the same as in the clock
 * @param  unix_time unix time to compare
 * @return           true if matches
 */
bool CTime::match(unsigned long unix_time)
{
  if(unix_time == get())
  {
    return true;
  }
  else
  {
    return false;
  }
}
void CTime::update(unsigned long unix_time)
{
  DateTime *currentTime = new DateTime(unix_time);
  _time = *currentTime;

  //Why all this :c
  //i'd like to do _rtc->setTime(DateTime)
  //TODO : calculate and correct offset of seconds
  _rtc->setSecond(currentTime->second());
  _rtc->setYear(currentTime->year());
  _rtc->setMonth(currentTime->month());
  _rtc->setDate(currentTime->day());
  _rtc->setDoW(currentTime->dayOfTheWeek());
  _rtc->setHour(currentTime->hour());
  _rtc->setMinute(currentTime->minute());

  delete currentTime;
}
unsigned long CTime::toUnixTime(DateTime t)
{
  return t.unixtime();
}
void CTime::setTime(String timeStr)
{

}
/**
 * Get arguments from a command
 * @param  cmdInput    command
 * @param  argumentRow what argument nbr ? (0 is the first)
 * @return             argument requested
 */
String CTime::cutCmd(String cmdInput, int argumentRow)
{
  String textToReturn = "";

  //Iterate over characters
  int i = 0;
  //Iterate over words
  int i2 = 0;

  int maxLength = cmdInput.length();

  while(i2 != argumentRow)
  {
    textToReturn = "";
    while(cmdInput[i] != '\0' && cmdInput[i] != 32)
      {
        textToReturn += cmdInput[i];
        i++;
        if(i > maxLength)
        {
          return String("Command is too long");
        }
      }
      i++;
      //Skip the spaces
      while(cmdInput[i] == 32)
        {
          i++;
        }
      i2++;
  }
  return textToReturn;
}
CTime::~CTime()
{
  delete _rtc;
}
