#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H

/*
Protocol specifications :
 Without this protocol, the transmitter broadcasts commands to every receiver
 The transmitter can send commands to one specific receiver
 Each receiver has an address defined by deviceAddress in class declaration
 The maximum number of devices is defined by CH_MAX_DEVICE_ADDRESS_LEN
 Each receiver can send messages / commands to the transmitter
 by using a letter as the device address, this letter is 'm'

 This is what a command from transmitter looks like :
    "56/do something\r"
    56 -> Device address
    do something -> command destined to the specific device
    '/' -> in between character
    \n -> endline character
    So any command must look like this :
    <device_address>/<command>
    What if there is only one device on the network ?
    You could send a command like this : <command>
    Without the '/' ; the device id is set to 0

  A message from any receiver to the transmitter must be like this :
    "m/i cannot do something\r"
  All devices will receive the message but will ignore it
 */

#include <Arduino.h>
#include <SoftwareSerial.h>

#include <Logging.h>

/*
  Maximum device index length, passed that value, device index will become 0
  eg : if that value equals 3, then, your device index should never exceed 999
    999 -> 3 characters
    1000 -> 4 characters
  Note : The higher the value is, the higher the processing time will be
 */

#define CH_MAX_DEVICE_ADDRESS_LEN 3

#define LOCAL 0
#define RF 1

class CommunicationHandler
{
public:
  CommunicationHandler(HardwareSerial *s, SoftwareSerial *rf, Logger *log, unsigned int deviceAddress);

  void handler();
  ~CommunicationHandler();
private:

  void localSerialHandler();
  void rfSerialHandler();
  
  void processCommand(String cmd, int from);
  String extractCommand(String cmd);
  int finddeviceAddress(String s);

  HardwareSerial *_localSerial;
  SoftwareSerial *_rfSerial;

  String _localSerialMessage = "";
  String _rfSerialMessage = "";

  Logger *_log;

  unsigned int _deviceAddress;
};

#endif
