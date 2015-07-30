#ifndef ACTOR_H
#define ACTOR_H

#include "headers.h"

class Actor
{
public:
    Actor();
    Actor(unsigned int id);

    void set_id(unsigned int id);
    unsigned int get_id();
private:
    unsigned int id;
};

#endif // ACTOR_H
