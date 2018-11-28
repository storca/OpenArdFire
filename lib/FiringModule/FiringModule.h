#ifndef FIRING_MODULE_H
#define FIRING_MODULE_H

#include <Arduino.h>
#include <Radio.h>
#include <Message.h>
#include <Cues.h>
#include <Show.h>
#include <Tester.h>
#include <Module.h>
//Generated with helper
#include "ErrorCodes.h"

//Beta
#define FM_VERSION 0
class FiringModule : public Module
{
public:
    //Methods
    FiringModule(int address, int usable_cues, int test_pin, Cues::Relay *testRelay);
    void send(int code);
    void send(Message msg);
    void processMessage(Message msg);
    Cues* getCues();
    ~FiringModule();

    //Commands
    void info();
    void selfcheck();
    void testCues(Message msg);

    //Constants
    const int _address;
    const int _usable_cues;
    const int _test_pin;

    //Cue-related objects
    struct Cues::Relay *_test_relay;
    Cues *_cues;
    Show *_show;

    //Custom objects
    CRadio *_radio;
    Message *_mymsg;
    Tester *_tester;

private:
  void handler();
  String splitstr(String s, int i);

};

#endif
