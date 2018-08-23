#include "Message.h"
/*
Message Class
 */
/**
 * New message object
 * @param msg Message to process
 */
Message::Message(unsigned int deviceAddress)
{
  _deviceAddress = deviceAddress;
}
/**
 * Set encoded message to decode
 * @param msg Message to process
 */
void Message::setMessage(String msg)
{
  _msgToProcess = msg;
  process();
}
/**
 * Encode a command
 * @param msg      command to encode
 * @param receiver destination
 */
void Message::setCommand(String command, String receiver)
{
  _msgToProcess = encodeMessage(command, receiver);
  process();
}
/**
 * Get message that has been processed
 * @return Message that has been processed
 */
String Message::getMessage()
{
  return _msgToProcess;
}
/**
 * Encode a message with the protocol
 * @param  command  Command to send with its arguments
 * @param  receiver Receiver address
 * @return          Encoded message
 */
String Message::encodeMessage(String command, String receiver)
{
  String result = "";

  //Add our sender address
  result + String(_deviceAddress);
  //Add separation char
  result += '/';
  //Add receiver address
  result + receiver;
  //Add separation char
  result += ':';
  //Add command
  result + command;
  //Add endline char
  result += '\n';

  return result;
}
/**
 * Process the given message
 * @param maxMsgLen Max message length
 */
void Message::process(int maxMsgLen)
{
  int i = 0;
  while(_msgToProcess[i] != '/')
  {
    //Add character to from
    msg.from += _msgToProcess[i];
    i++;

    if(i >= maxMsgLen)
    {
      msg.from = "\0";
      return;
    }
  }

  //Skip the '/' character
  i++;

  while(_msgToProcess[i] != ':')
  {
    msg.to += _msgToProcess[i];
    i++;

    if(i >= maxMsgLen)
    {
      msg.to = "\0";
      return;
    }
  }

  //Skip the ':' character
  i++;

  //Rest of the string is the command
  while(_msgToProcess[i] != '\0')
  {
    msg.command += _msgToProcess[i];
    if(i >= maxMsgLen)
    {
      //message too long
      return;
    }
  }
}

/**
 * Take arguments from command
 * @param  argument argument row
 * @return          selected argument if available
 */
String Message::splitCommand(int argument)
{
  String command = msg.command;
  String textToReturn = "";

  //Iterate over characters
  int i = 0;
  //Iterate over words
  int i2 = 0;

  int maxLength = command.length();

  while(i2 != argument)
  {
    textToReturn = "";
    while(command[i] != '\0' && command[i] != 32)
      {
        textToReturn += command[i];
        i++;
        if(i > maxLength)
        {
          return String("Command is too long");
        }
      }
      i++;
      //Skip the spaces
      while(command[i] == 32)
        {
          i++;
        }
      i2++;
  }
  return textToReturn;
}

Message::~Message()
{
}
