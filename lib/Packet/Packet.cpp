#include "Packet.h"

Packet::Packet()
{
    cursor = -1;
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
bool Packet::append_data(char d)
{
    if(3+cursor++ < RF_PACKET_SIZE)
    {
        cursor++;
        _buffer[cursor+3] = d;
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Encodes a given long variable into 4 bytes
 * 
 * @param bytearr Buffer of 4 bytes long
 * @param var Long variable to encode
 */
void Packet::encode_long(char* bytearr, long var)
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
void Packet::decode_long(char* bytearr, long *var)
{
    *var = (unsigned long)(bytearr[3] << 24) | (bytearr[2] << 16) | (bytearr[1] << 8) | bytearr[0];
}

/**
 * @brief Encodes a given int to 2 bytes
 * 
 * @param bytearr Buffer to store the integer
 * @param var Integer to encode
 */
void Packet::encode_int(char* bytearr, int var)
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
void Packet::decode_int(char* bytearr, int *var)
{
    *var = (unsigned int)(bytearr[1] << 8) | bytearr[0];
}