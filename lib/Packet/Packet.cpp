#include "Packet.h"

/**
 * @brief Construct a new Packet:: Packet object
 * Write mode is the default mode
 * 
 */
Packet::Packet()
{
    beginWrite();
}
/**
 * @brief Construct a new Packet:: Packet object
 * with the given mode
 * 
 * @param mode The mode to set using Packet::Mode enum
 */
Packet::Packet(bool mode)
{
    setMode(mode);
}

/**
 * @brief Set packet either to write or read mode
 * Using the Packet::Mode enum
 * 
 * @param mode Enum type
 */
void Packet::setMode(bool mode)
{
    switch(mode)
    {
        case Packet::Mode::Read:
            beginRead();
        break;
        case Packet::Mode::Write:
            beginWrite();
        break;
    };
}

/**
 * @brief Sets the receiver address
 * 
 * @param s 
 */
void Packet::sender(char s)
{
    _buffer[1] = s;
}
/**
 * @brief Returns a pointer to the sender address
 * 
 * @return char Pointer to the sender address
 */
char* Packet::sender()
{
    return &_buffer[1];
}

/**
 * @brief Sets the receiver address
 * 
 * @param r Receiver address
 */
void Packet::receiver(char r)
{
    _buffer[2] = r;
}
/**
 * @brief Returns a pointer to the receiver address
 * 
 * @return char* Pointer to the receiver address
 */
char* Packet::receiver()
{
    return &_buffer[2];
}

/**
 * @brief Returns the entire buffer of the packet
 * 
 * @return char* Pointer to the buffer
 */
char* Packet::get_buffer()
{
    return _buffer;
}
/**
 * @brief Appends data to the buffer
 * 
 * @param d byte to append
 * @return true The byte has been appened
 * @return false The byte could not be appened
 */
bool Packet::append(char d)
{
    if(_mode == Packet::Mode::Write)
    {
        if(3+cursor++ < RF_PACKET_SIZE)
        {
            cursor++;
            //Skip the 3 info bytes and write into the current cursor position
            _buffer[cursor+3] = d;
            //Increase packet size
            _buffer[0]++;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        throw "Packet is in read mode and you are trying to write into it";
        return false;
    }
}

/**
 * @brief Encodes a given long variable into 4 bytes
 * 
 * @param bytearr Buffer of 4 bytes long
 * @param var Long variable to encode
 */
void Packet::encode_long(char* bytearr, uint32_t var)
{
    bytearr[0] = (char) var;
    bytearr[1] = (char) var >> 8;
    bytearr[2] = (char) var >> 16;
    bytearr[3] = (char) var >> 24;
}
/**
 * @brief Decodes any given 4 bytes into a long variable
 * 
 * @param bytearr Byte array to decode
 * @param var Buffer for the long variable
 */
void Packet::decode_long(char* bytearr, uint32_t *var)
{
    *var = (uint32_t)(bytearr[3] << 24) | (bytearr[2] << 16) | (bytearr[1] << 8) | bytearr[0];
}

/**
 * @brief Encodes a given int to 2 bytes
 * 
 * @param bytearr Buffer to store the integer
 * @param var Integer to encode
 */
void Packet::encode_int(char* bytearr, uint16_t var)
{
    bytearr[0] = (char) var;
    bytearr[1] = (char) var >> 8;
}
/**
 * @brief Decodes any given 2 bytes into an integer
 * 
 * @param bytearr Byte array to decode
 * @param var Buffer for the integer variable
 */
void Packet::decode_int(char* bytearr, uint16_t *var)
{
    *var = (uint16_t)(bytearr[1] << 8) | bytearr[0];
}

/**
 * @brief Sets the packet object in writing mode
 * 
 */
void Packet::beginWrite()
{
    //Set the cursor to -1 so the function append(char) could work properly
    cursor = -1;
    //Set the buffer size to 0
    _buffer[0] = 0;
}

/**
 * @brief Checks if there is enough space to write a given type
 * 
 * @tparam T Type to write
 * @return true There is enough space
 * @return false There is no much space :c
 */
bool Packet::hasEnoughSpace(unsigned int size)
{
    if(cursor + size < RF_PACKET_SIZE)
    {
        return true;
    }
    return false;
}

/**
 * @brief Appends an integer to the packet
 * 
 * @param var Integer to append
 * @return true 
 * @return false 
 */
bool Packet::append(uint16_t var)
{
    char buff[2];

    encode_int(buff, var);

    for(int i =0; i<2; i++)
    {
        if(!append(buff[i]))
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Appends a long to the packet
 * 
 * @param var Long to append
 * @return true 
 * @return false 
 */
bool Packet::append(uint32_t var)
{
    char buff[4];

    encode_long(buff, var);

    for(int i =0; i<4; i++)
    {
        if(!append(buff[i]))
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Sets the packet object in reading mode
 * 
 */
void Packet::beginRead()
{
    //Set the cursor so the read function could work as intended
    //Cursor is 1 here because we only read data
    //It will be incremented in read() then the array value will be returned
    cursor = 1;
}

/**
 * @brief How much bytes are available for reading<br>
 * Info bytes not included
 * 
 * @return char Bytes left to read
 */
char Packet::available()
{
    if(_mode == Packet::Mode::Read)
    {
        return _buffer[0] - cursor;
    }
    else
        return 0;
}

/**
 * @brief Read a single byte from the packet
 * 
 * @return char 
 */
char Packet::read()
{
    if(_mode == Packet::Mode::Read)
    {
        if(cursor++ < RF_PACKET_SIZE)
        {
            cursor++;
            return _buffer[cursor];
        }
        //Overflowwwww yay
        return false;
    }
    else
    {
        throw "Packet is in write mode and you are trying to read contents of it";
        return false;
    }
}
/**
 * @brief Reads an integer from the packet
 * 
 * @param var 
 */
void Packet::read(uint16_t *var)
{
    //Create a buffer
    char buff[2];
    //Copy contents from the packet to the buffer
    for(int i=0; i<2; i++)
    {
        buff[i] = read();
    }
    //Decode the variable
    decode_int(buff, var);
}
/**
 * @brief Reads a long from the packet
 * 
 * @param var 
 */
void Packet::read(uint32_t *var)
{
    //Create a buffer
    char buff[4];
    //Copy contents from the packet to the buffer
    for(int i=0; i<4; i++)
    {
        buff[i] = read();
    }
    //Decode the variable
    decode_long(buff, var);
}
