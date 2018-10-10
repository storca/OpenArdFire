#include <Arduino.h>
#include <Radio.h>

//New radio on channel 5
CRadio rad(5);

void setup()
{
    rad.begin();
}

void loop()
{
    rad.handler();
}