#ifndef PACKET_H
#define PACKET_H

#ifndef RF_PACKET_SIZE
#define RF_PACKET_SIZE 32
#endif


/**
 * @brief Parent class of Command class
 * Written in c++ ; so no arduino types
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
        bool append(int);
        bool append(long);

        void beginRead();
        char read();
        void read(int*);
        void read(long*);

        char* get_buffer();

        static void encode_long(char[4], long);
        static void decode_long(char[4], long*);

        static void encode_int(char[2], int);
        static void decode_int(char[2], int*);

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