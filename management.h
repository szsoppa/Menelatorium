#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "communicator.h"
#include "lamport.h"

class Management : public Communicator, public Lamport
{
public:
    Management(int positions_number, MPI_Comm COMM_WORLD);

    void participate();

private:
    int positions_number;
    int world_group_size;
    MPI_Comm COMM_MENELS, COMM_WORLD;
    Packet p;
};

#endif // MANAGEMENT_H
