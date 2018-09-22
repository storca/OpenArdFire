#include <Arduino.h>
#include <Radio.h>
#include <Message.h>
#include <Cues.h>
#include <Show.h>
//Generated with helper
#include "ErrorCodes.h"

class FiringModule
{
public:
    //Methods
    FiringModule(int address);
    void send(int code);
    void send(Message msg);
    void processMessage(Message msg);
    Cues* getCues();
    ~FiringModule();

    //Commands
    void info();
    void selfcheck();
    void testCues(Message msg);

    //Constants FIXME: actually use constants
    int _address;
    int _usable_cues;

    //Cue-related objects
    Cues *_cues;
    Show *_show;

    //Communication object
    CRadio *_radio;

    //Securities
    bool test_authorised = false;
    bool show_authorised = false;

private:
  void handler();
  String splitCommand();

}
