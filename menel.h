#ifndef MENEL_H
#define MENEL_H

#include "communicator.h"
#include "lamport.h"

class Menel : public Communicator, public Lamport
{
public:
    Menel();
    Menel(unsigned int id) : Communicator(), Lamport()
    {}
};

#endif // MENEL_H
