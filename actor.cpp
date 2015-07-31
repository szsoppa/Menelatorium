#include "actor.h"

Actor::Actor()
{
    this->timestamp = 0;
}

void Actor::set_world_id(unsigned int id)
{
    this->world_id = id;
}

unsigned int Actor::get_world_id()
{
    return this->world_id;
}

void Actor::set_group_id(unsigned int id)
{
    this->group_id = id;
}

unsigned int Actor::get_group_id()
{
    return this->group_id;
}

void Actor::set_timestamp(int timestamp)
{
    this->timestamp = timestamp;
}

int Actor::get_timestamp()
{
    return this->timestamp;
}
