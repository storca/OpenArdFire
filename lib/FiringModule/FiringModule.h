#include <Arduino.h>
#include <Radio.h>
#include <Message.h>
#include <Cues.h>
#include <Show.h>
//Generated with helper
#include "ErrorCodes.h"

#ifndef FIRING_MODULE_H

#define FIRING_MODULE_H
//Beta
#define FM_VERSION 0
class FiringModule
{
public:
    //Methods
    FiringModule(int address, int usable_cues);
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
    const int _usable_cues;

    //Cue-related objects
    Cues *_cues;
    Show *_show;

    //Communication object
    CRadio *_radio;
    Message *_mymsg;

    //Securities
    bool test_authorised = false;
    bool show_authorised = false;

    //Status Variables
    bool show_running = false;

private:
  void handler();
  String splitstr(String s, int i);

};

#endif
