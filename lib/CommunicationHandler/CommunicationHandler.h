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
    "m/56:do something\r"
    m -> sender
    56 -> receiver
    do something -> command destined to the specific device
    '/' -> means "sends to"
    ':' -> after that character is the command
    \n -> endline character
    So any command must look like this :
    <sender>/<device_address>:<command>
    What if there is only one device on the network ?
    Syntax must be respected, otherwise, command will be ignored

  A message from any receiver to the transmitter must be like this :
    "23/m:i cannot do something\r"
  All devices will receive the message but will ignore it
  NOTE : all treatment is done by
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
#define CH_MAX_MESSAGE_LEN 256

#define CH_ENDL_CHAR '\n'

//Used for messages provenance in processCommand
enum {Local, Radio, Both};

//Class used to decode and address messages
//The processing is a bit heavy, this is why I created a separated class
class Message
{
public:
  struct message
  {
    String from;
    String to;
    String command;
  };
  struct message *msg;

  Message();
  void setMessage(String msg);
  String getMessage();
  ~Message();

private:
  //Struct to store messages
  void process(int maxMsgLen = CH_MAX_MESSAGE_LEN);
  String _msgToProcess;

};

class CommunicationHandler
{
  //TODO : RTS and CTS messages to avoid data collision
public:
  CommunicationHandler(HardwareSerial *s, SoftwareSerial *rf, unsigned int deviceAddress);

  void handler();
  void print(int errorCode, int to);
  void print(const char *msg, int to);

  String getCommand(int from);

  ~CommunicationHandler();
private:

  void localSerialHandler();
  void rfSerialHandler();

  bool needSend(String *buffer);

  void processMessage(String message, int from);

  HardwareSerial *_localSerial;
  SoftwareSerial *_rfSerial;

  String _localSerialMessage = "";
  String _rfSerialMessage = "";

  String _localSerialBuffer = "";
  String _rfSerialBuffer = "";

  String _localProcessQueue = "";
  String _rfProcessQueue = "";

  Message *_messageHandler;

  unsigned int _deviceAddress;
};


#endif
