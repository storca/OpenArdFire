#include <Arduino.h>
#include <Packet.h>

Packet myPacket;

void setup()
{
    Serial.begin(9600);
    myPacket.setMode(Packet::Mode::Write);
    //Master
    myPacket.sender(0);
    //Broadcast
    myPacket.receiver(255);

    //Write things into it
    myPacket.append(uint32_t(4294967275));
    myPacket.append(uint16_t(65530));

    char i = 0;
    while(myPacket.hasEnoughSpace(sizeof(char)))
    {
        myPacket.append(i);
        i++;
    }

    //Read those things back
    myPacket.setMode(Packet::Mode::Read);

    //Read 32 bits variable
    uint32_t var32;
    myPacket.read(&var);
    Serial.println(var32, HEX);
    Serial.println(var32);

    //Read 16 bits variable
    uint16_t var16;
    myPacket.read(&var16);
    Serial.println(val, HEX);
    Serial.println(var16)

    //Start reading from the begining again
    myPacket.beginRead();

    while(myPacket.available())
    {
        Serial.print(myPacket.read(), HEX);
    }

}

void loop()
{
    //Nothing to do here
}