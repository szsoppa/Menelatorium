#ifndef ACTOR_H
#define ACTOR_H

#include "headers.h"

class Actor
{
public:
    Actor();
    Actor(unsigned int id);

    void set_id(unsigned int id);
    void set_timestamp(int timestamp);
    unsigned int get_id();
    int get_timestamp();
private:
    unsigned int id;
    int timestamp;
};

#endif // ACTOR_H
