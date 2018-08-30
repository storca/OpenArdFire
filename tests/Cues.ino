#include <Cues.h>

//Put mcp addresses here
//By default 0x20 and 0x21
#define C_MCPS {0x25, 0x26, 0x27}

//Let's say that we'd like to use 35 cues out of the 48 available
Cues myCues(35);

void setup()
{
  //In milliseconds, by default 500ms
  myCues._onTime = 2000;

  //Authorise cue triggering
  myCues.authorise(true);

}

void loop()
{
  //Call the handler
  myCues.handler();

  //Those two cues will be almost triggered together
  //and then turned off by handler().
  //it will not slow down the loop as it does
  //not work with delay()
  myCues.trigger(1);
  myCues.trigger(4);

  //The second cue will not be triggered
  //because we enabled the security
  myCues.authorise(false);
  myCues.trigger(2);

}
