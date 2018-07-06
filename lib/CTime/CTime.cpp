#include "CTime.h"

CTime::CTime()
{
  DS3231_init(DS3231_INTCN);
}
/**
 * Get the actual unix time
 * @return Unix time
 */
unsigned long CTime::get()
{
  DS3231_get(&(this->_time));
  return this->_time.unixtime;
}
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
