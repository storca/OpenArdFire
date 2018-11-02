#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>
#include <Option.h>

#define MD_PACKET_SIZE 32

/**
 * @brief Parent class of all modules<br>
 * - Keeps options<br>
 * - Handles human-machine interface such as leds, buttons, lcd etc<br>
 * - Adds flexibility to the system, for example, the user could choose
 * whether or not his submodule needs options, if yes, then he calls setOptions()<br>
 * 
 * It's also where the protocol is implemented, it replaces Message class
 */
class Module
{
    public:
        Module();
        byte* packet(byte sender, byte receiver, byte *message);

        //Options
        void setOptions(AbstractOption *options);
        AbstractOption* getOptions();

        //Virtual dtor because it's the base class
        virtual ~Module();
        
    private:
        byte _packet_data[MD_PACKET_SIZE];
        //Options
        bool _options_used = false;
        AbstractOption *_options = nullptr;

};

#endif