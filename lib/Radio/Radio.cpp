#include "Radio.h"
/**
 * Radio Ctor
 */
Radio::Radio()
{
  _sendBuffer = new String();
  _receiveBuffer = new String();
}
/**
 * Initialise radio on a agiven channel
 * @param  rfchannel channel
 * @return           true if success
 */
bool Radio::begin(uint8_t rfchannel)
{
  if (rfchannel > -1 && rfchannel < 256)
  {
    channel = rfchannel;
    Mirf.cePin = 9;
    Mirf.csnPin = 10;
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();

    Mirf.channel = channel;
    //32o
    Mirf.payload = RF_PACKET_SIZE;
    Mirf.config();

    setReceiveAddress(rfchannel);

    return true;
  }
  return false;
}
/**
 * Handles communications
 */
void Radio::handler()
{
  if(!Mirf.isSending() && !Mirf.dataReady() && !ready())
  {
    //We can send data
    byte packet[RF_PACKET_SIZE];

    for (size_t i = 0; i < RF_PACKET_SIZE+1; i++) {
      packet[i] = int(_sendBuffer->charAt(i));
    }
    //Remove packet size from buffer
    _sendBuffer->remove(0, RF_PACKET_SIZE);
    //Send the packet
    Mirf.send(packet);
  }
  else if(Mirf.dataReady())
  {
    //We can receive data
    byte packet[RF_PACKET_SIZE];
    Mirf.getData(packet);
    processPacket(packet);
  }
}
//NOTE : The two next funtions are here to
//handle addresses greater than 255 if needed
void setSendAddress(uint8_t receiver)
{
  Mirf.setTADDR(&receiver);
}
void setReceiveAddress(uint8_t me)
{
  Mirf.setRADDR(&me);
}
/**
 * Checks if the radio is ready to send
 * @return true if true :p
 */
bool Radio::ready()
//Just checks if the buffer is empty
{
  if(*_sendBuffer == "")
  {
    return true;
  }
  else
  {
    return false;
  }
}
/**
 * Process a given packet
 * @param packet packet to process
 */
void Radio::processPacket(byte packet[RF_PACKET_SIZE])
{
  //Add the packet to the receive buffer
  for (size_t i = 0; i < RF_PACKET_SIZE+1; i++) {
    *_receiveBuffer += char(packet[i]);
  }
}
/**
 * Checks if radio is able to give commands
 * @return true if true
 */
bool Radio::available()
{
  if(*_receiveBuffer == "")
    return false;
  else
    return true;
}
/**
 * Get a command from radios
 * @return command
 */
String Radio::getCommand()
{
  size_t i = 0;
  String command;
  while(_receiveBuffer->charAt(i) != char(RF_ENDMSG_CHAR))
  {
    command += char(_receiveBuffer->charAt(i));
    if(i > 255)
    {
      return String("");
    }
    i++;
  }
  //Skip end msg character to remove it
  i++;
  _receiveBuffer->remove(0, i);
  return command;
}
/**
 * Send a command on a given receiver
 * @param command  command to send
 * @param receiver receiver's address
 */
void Radio::send(String command, uint8_t receiver)
{
  if(!ready())
    return;

  setSendAddress(receiver);
  //Append command to buffer
  *_sendBuffer = command;
  *_sendBuffer += char(RF_ENDMSG_CHAR);
}
/**
 * Radio Dtor
 */
Radio::~Radio()
{
  delete _sendBuffer;
  delete _receiveBuffer;
}
