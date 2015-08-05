#ifndef ACTOR_H
#define ACTOR_H

#include "headers.h"
#include "constants.h"
#include "packet.h"

class Actor
{
public:
    Actor();

    void set_world_id(unsigned int id);
    int get_world_id();
    void set_group_id(unsigned int id);
    int get_group_id();
    void set_timestamp(int timestamp);
    int get_timestamp();
private:
    int world_id;
    int group_id;
    int timestamp;
};

#endif // ACTOR_H
