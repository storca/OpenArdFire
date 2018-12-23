#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#ifndef RF_PACKET_SIZE
#define RF_PACKET_SIZE 32
#endif

/**
 * @brief Parent class of Command class
 * Handles packet decoding and encoding<br>
 * Supposed to be cross-platform (used in the software in the future)
 * 
 */
class Packet
{
    
    public:

        enum Mode
        {
            Read,
            Write
        };

        Packet();
        Packet(bool mode);
        virtual ~Packet();

        void setMode(bool);

        void sender(char);
        char* sender();

        void receiver(char);
        char* receiver();

        void beginWrite();
        
        bool hasEnoughSpace(unsigned int);
        bool append(char);
        bool append(uint16_t);
        bool append(uint32_t);

        void beginRead();
        char read();
        void read(uint16_t*);
        void read(uint32_t*);

        char* get_buffer();

        static void encode_long(char[4], uint32_t);
        static void decode_long(char[4], uint32_t*);

        static void encode_int(char[2], uint16_t);
        static void decode_int(char[2], uint16_t*);

        /**
         * @brief Variable used to store to previously changed element
         * 
         */
        int cursor;

    private:
        char _buffer[RF_PACKET_SIZE];
        bool _mode;
        
};

#endif