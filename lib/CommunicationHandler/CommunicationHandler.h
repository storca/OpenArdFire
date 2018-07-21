#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H

/*
This library handles PC to receiver communications
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
    <sender>/<receiver>:<command>\n
    What if there is only one device on the network ?
    Syntax must be respected, otherwise, command will be ignored

  A message from any receiver to the transmitter must be like this :
    "23/m:i cannot do something\r"
  All devices will receive the message but will ignore it
  NOTE : receivers are able to receive messages only from 'm'
  NOTE : receiver address 'a' means all ; so 'm/a:selftest' will be sent
  to all receivers
 */

#include <Arduino.h>
#include <SoftwareSerial.h>

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

  Message(unsigned int *deviceAddress);
  void setMessage(String msg);
  String getMessage();
  String encodeMessage(String command, String receiver);
  ~Message();

private:
  //Struct to store messages
  void process(int maxMsgLen = CH_MAX_MESSAGE_LEN);
  String _msgToProcess;
  unsigned int *_deviceAddress = nullptr;

};

class CommunicationHandler
{
  //TODO : RTS and CTS messages to avoid data collision
public:
  CommunicationHandler(HardwareSerial *s, SoftwareSerial *rf, unsigned int deviceAddress);

  void handler();
  //Input functions
  void print(int errorCode, int to);
  void print(const char *msg, int to);

  //Output function
  String getCommand(int from);

  ~CommunicationHandler();
private:

  bool isAddressInt(String address);

  void localSerialHandler();
  void rfSerialHandler();

  String processSend(String *queue);
  bool needSend(String *buffer);

  String extractCommand(String *commandQueue);

  void processMessage(String message, int from);

  HardwareSerial *_localSerial;
  SoftwareSerial *_rfSerial;

  //Raw received data from serial ports
  String _localSerialMessage = "";
  String _rfSerialMessage = "";

  //Data that is waiting to be sended
  String _localSerialBuffer = "";
  String _rfSerialBuffer = "";

  //Received messages to process
  String _localProcessQueue = "";
  String _rfProcessQueue = "";

  Message *_messageHandler;

  unsigned int _deviceAddress;
};


#endif
