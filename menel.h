#ifndef MENEL_H
#define MENEL_H

#include "communicator.h"
#include "lamport.h"

class Menel : public Communicator, public Lamport
{
public:
    Menel();
    Menel(int positions_number, MPI_Comm COMM_WORLD, MPI_Comm COMM_MENELS);

    void participate();
    int get_positions_number();

private:
    int weight;
    int management_id;
    int world_group_size;
    int menel_group_size;
    int positions_number;
    vector<int> menels;
    MPI_Comm COMM_MENELS, COMM_WORLD;
    Packet p;

    void enter_exhibition();
    void read_by_tag(int tag);
    int get_random(int lower, int upper);
};

#endif // MENEL_H
