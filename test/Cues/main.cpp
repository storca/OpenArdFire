//Set mcp addresses before including library
//By default 0x20 and 0x21
#define C_MCPS {0x25, 0x26, 0x27}

#include <Cues.h>


//Let's say that we'd like to use 35 cues out of the 48 available
Cues *myCues;

//Set our test relay
struct Cues::Relay myRelay = {9, LOW};

void setup()
{

  //New object
  myCues = new Cues(35, &myRelay);

  //In milliseconds, by default 500ms
  myCues->_onTime = 2000;
  
  myCues->begin_test();
  myCues->test(34, HIGH);
  //This is where we use digitalRead() function
  //In order to know if the current if flowing through the igniter
  delay(1000);
  myCues->test(34, LOW);
  myCues->end_test();

  //Authorise cue triggering
  myCues->authorise(true);

  //Those two cues will be almost triggered together
  //and then turned off by handler().
  //it will not slow down the loop as it does
  //not work with delay()
  myCues->trigger(1);
  myCues->trigger(4);

  //The second cue will not be triggered
  //because we enabled the security
  myCues->authorise(false);
  myCues->trigger(2);

}

void loop()
{
  //Call the handler
  myCues->handler();
  
  delete myCues;
}
