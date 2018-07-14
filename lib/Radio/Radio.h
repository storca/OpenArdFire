/**
 * This class handles NRF24L01 communications with Mirf library
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

//How much bytes in a packet
#define RF_PACKET_SIZE 16
//What is the 'end message' character
#define RF_ENDMSG_CHAR 4


class Radio
{
public:
  Radio();
  bool begin(uint8_t rfchannel);
  void handler();
  void send(String command, uint8_t receiver);
  String getCommand();
  bool ready();
  bool available();
  ~Radio();

  //FIXME : has to be a constant
  uint8_t channel;
private:
  void processPacket(byte packet[RF_PACKET_SIZE]);
  void setSendAddress(uint8_t receiver = 1);
  void setReceiveAddress(uint8_t me = 1);
  //Buffer used to stack messages before sending
  String *_sendBuffer;
  String *_receiveBuffer;
};
