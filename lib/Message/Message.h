#ifndef COMMAND_H
#define COMMAND_H

#include <Arduino.h>

#define MSG_MAX_MESSAGE_LEN 256

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
  struct message msg;

  unsigned int deviceAddress;

  Message(unsigned int deviceAddress);
  void setMessage(String msg);
  void setCommand(String command, String receiver);
  String getMessage();
  String encodeMessage(String command, String receiver);
  String splitCommand(int argument);
  ~Message();

private:
  //Struct to store messages
  void process(int maxMsgLen = MSG_MAX_MESSAGE_LEN);
  String _msgToProcess;
  unsigned int _deviceAddress;

};

#endif
