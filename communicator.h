#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "actor.h"

class Communicator : public virtual Actor
{
public:
    Communicator();
    void send(int *buff, int count, int destiny , int tag, MPI_Comm comm);
    void read(int *buff, int count, int source , int tag, MPI_Comm comm);
    void read(int *buff, int count, int source , int tag, MPI_Comm comm, MPI_Status *status);
    void broadcast(int *buff, int count, int tag, MPI_Comm comm);
private:
    void increment_timestamp();
};

#endif // COMUNICATOR_H
