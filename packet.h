#ifndef PACKET_H
#define PACKET_H

#include "headers.h"

class Packet
{
public:
    Packet();

    void set(int timestamp, int message);
    void set_timestamp(int timestamp);
    void set_message(int message);
    int get_timestamp();
    int get_message();
    int get_size();
    int *get();

private:
    int size;
    int data[2];
};

#endif // PACKET_H
