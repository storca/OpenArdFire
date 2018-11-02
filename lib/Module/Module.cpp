#include "Module.h"

Module::Module()
{

}

byte* Module::packet(byte sender, byte receiver, byte* message)
{
    //Let's find the size of the parameter
    size_t msg_len = sizeof(*message)/sizeof(byte);

    //TODO : check message length

    //Actually create the packet
    //3 info bytes
    //length -- sender -- receiver -- message
    size_t packet_size = msg_len + 3;

    //Encode things
    _packet_data[0] = MD_PACKET_SIZE;
    _packet_data[1] = sender;
    _packet_data[2] = receiver;
    //Add message to the packet
    for(size_t i=3; i<packet_size; i++)
    {
        _packet_data[i] = message[i-3];
    }

    return _packet_data;
}

void Module::setOptions(AbstractOption *options)
{
    _options_used = true;
    _options = options;
}

AbstractOption* Module::getOptions()
{
    return _options;
}

Module::~Module()
{
    if(_options_used)
    {
        delete _options;
    }
}