#ifndef LAMPORT_H
#define LAMPORT_H

#include "actor.h"

class Lamport : public Actor
{
public:
    Lamport();
    
private:
    unsigned int timestamp;
};

#endif // LAMPORT_H
