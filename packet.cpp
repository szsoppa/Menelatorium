#include "packet.h"

Packet::Packet()
{
    size = 2;
}

void Packet::set(int timestamp, int message)
{
    set_timestamp(timestamp);
    set_message(message);
}

void Packet::set_timestamp(int timestamp)
{
    data[0] = timestamp;
}

void Packet::set_message(int message)
{
    data[1] = message;
}

int Packet::get_timestamp()
{
    return data[0];
}

int Packet::get_message()
{
    return data[1];
}

int Packet::get_size()
{
    return size;
}

int *Packet::get()
{
    return data;
}
