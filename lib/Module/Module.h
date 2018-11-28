/**
 * @file Module.h
 * @author Storca (storca@mail.com)
 * @brief Parent class for all modules
 * @version 0.1
 * @date 2018-11-04
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>
#include <Option.h>

#include "options.h"
//Static options below, may be used in a separate file in the future
//I use macros here to reduce memory usage and compilation time
//in case those options are not used

//Could be string or number
#define MD_VERSION "dev"

#ifndef MD_PACKET_SIZE
#define MD_PACKET_SIZE 32
#endif

//Do we use LCD screen
#ifdef MD_USE_LCD
    //Libraries used for 128x32 screen
    #include <SPI.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>

    #ifndef SSD1306_I2C_ADDRESS
        #define SSD1306_I2C_ADDRESS 0x3C
    #endif

    #define OLED_RESET 4
#endif

//Do we use 2.4GHz radio
#ifdef MD_USE_2_4_GHZ
    #include <Radio.h>
#endif

//Do we use long range modules
#ifdef MD_USE_433_MHZ
    #include <SoftwareSerial.h>
#endif


/**
 * @brief Parent class of all modules<br>
 * - Common base for all modules of the network (transcievers, receivers and firing modules)
 * - Keeps options<br>
 * - Handles human-machine interface such as leds, buttons, lcd etc<br>
 * - Adds flexibility to the system, for example, the user could choose
 * whether or not his submodule needs options, if yes, then he calls setOptions()<br>
 * 
 * It's also where the protocol is implemented, it replaces heavy duty Message class
 */
class Module
{
    public:
        Module();
        static bool encode_packet(byte *buff, byte sender, byte receiver, byte *message);
        static bool is_packet_valid(byte *packet);
        static bool decode_packet(byte *packet, byte *sender, byte *receiver, byte message[MD_PACKET_SIZE-3]);

        //Options
        void setOptions(AbstractOption *options);
        AbstractOption* getOptions();

        //Virtual dtor because it's the base class
        virtual ~Module();

        //Public members
        #ifdef MD_USE_LCD
            Adafruit_SSD1306 *_display;
        #endif

        #ifdef MD_USE_2_4_GHZ
            CRadio *_radio = nullptr;
        #endif
        
    private:
        void find_address();
        //Options
        bool _options_used = false;
        AbstractOption *_options = nullptr;

};

#endif