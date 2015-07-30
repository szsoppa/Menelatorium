#include "actor.h"

Actor::Actor()
{
}

Actor::Actor(unsigned int id)
{
    this->id = id;
}

void Actor::set_id(unsigned int id)
{
    this->id = id;
}

unsigned int Actor::get_id()
{
    return this->id;
}
