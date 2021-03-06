#include <Arduino.h>
#include <Cues.h>
#include <Show.h>

struct Cues::Relay myrelay = {9, HIGH};

Cues myCues(32, &myrelay);
Show myShow(&myCues);

void setup()
{
    //Cue 1 triggered at 1ms
    //Cue 2 triggered at 2ms
    //etc...
    for(size_t i=1; i<33; ++i)
    {
        myShow.set(i, i);
    }
    myShow.save(0);
    myShow.start();
}

void loop()
{
    myCues.handler();
    myShow.handler();
}