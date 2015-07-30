#include "actor.h"

Actor::Actor()
{
    this->timestamp = 0;
}

Actor::Actor(unsigned int id)
{
    this->id = id;
    this->timestamp = 0;
}

void Actor::set_id(unsigned int id)
{
    this->id = id;
}

void Actor::set_timestamp(int timestamp)
{
    this->timestamp = timestamp;
}

unsigned int Actor::get_id()
{
    return this->id;
}

int Actor::get_timestamp()
{
    return this->timestamp;
}
