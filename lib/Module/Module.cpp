#include "Module.h"

/**
 * @brief Construct a new Module:: Module object
 * The constructor depends on the macros in config.h
 * 
 */
Module::Module()
{
    //LCD
    #ifdef MD_USE_LCD
        _display = new Adafruit_SSD1306(OLED_RESET);

        _display->begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
        //TODO : review this
        String versionstr = "OpenArdFire " + String(MD_VERSION);
        _display->println(versionstr);
    #endif

    //Clear the display after things were set
    #ifdef MD_USE_LCD
        _display->clearDisplay();
    #endif
}

/**
 * @brief Encodes things to a packet of bytes
 * 
 * @param buff Buffer to store the packet
 * @param sender Sender address
 * @param receiver Receiver address
 * @param message Message to send
 * @return true Packet was encoded
 * @return false Packet could not be encoded due to a too long message for the packet size
 */
bool Module::encode_packet(byte *buff, byte sender, byte receiver, byte* message)
{
    //Let's find the size of the parameter
    size_t msg_len = sizeof(*message)/sizeof(byte);

    if(msg_len+3 > MD_PACKET_SIZE)
    {
        return false;
    }

    //Actually create the packet
    //3 info bytes
    //length -- sender -- receiver -- message
    size_t packet_size = msg_len + 3;

    //Encode things
    buff[0] = MD_PACKET_SIZE;
    buff[1] = sender;
    buff[2] = receiver;
    //Add message to the packet
    for(size_t i=3; i<packet_size; i++)
    {
        buff[i] = message[i-3];
    }
    return true;
}

/**
 * @brief Checks if the packet does not exceed MD_PACKET_SIZE
 * 
 * @param packet Packet to check
 * @return true Packet is able to be processed
 * @return false Packet is too long
 */
bool Module::is_packet_valid(byte *packet)
{
    //If packet does exceed maximum length
    if(packet[0] > MD_PACKET_SIZE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief Decodes a given packet, of maximum length defined by MD_PACKET_SIZE<br>
 * Important note : message ptr exists as soon as packet ptr exists
 * 
 * @param packet Given packet to decode, may be smaller than MD_PACKET_SIZE
 * @param sender From who the packet was sended
 * @param receiver To who the packet is destinated
 * @param message Message sended, begins with it's length
 * @return true Packet was decoded and available in message ptr
 * @return false Packet could not be decoded due to it's length
 */
bool Module::decode_packet(byte *packet, byte *sender, byte *receiver, byte message[MD_PACKET_SIZE-3])
{
    if(!is_packet_valid(packet))
    {
        return false;
    }
    else if(packet[0] == MD_PACKET_SIZE)
    {
        //Set sender and receiver values via pointers
        sender = &packet[1];
        receiver = &packet[2];
        //Set the message length witch is 3 bytes smaller than the packet
        message[0] = packet[0]-3;
        //Put packet data into the message pointer
        for(size_t i=2; i<MD_PACKET_SIZE-3; i++)
        {
            message[i-1] = packet[i];
        }
    }
    else if(packet[0] < MD_PACKET_SIZE)
    {
        //Same process
        sender = &packet[1];
        receiver = &packet[2];

        message[0] = packet[0]-3;
        //Same process except that the packet is smaller than expected
        for(size_t i=2; i<packet[0]-3; i++)
        {
            message[i-1] = packet[i];
        }
    }
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
    #ifdef MD_USE_LCD
        delete _display;
    #endif
}