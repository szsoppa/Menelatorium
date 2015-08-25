#ifndef STAFF_H
#define STAFF_H

#include "communicator.h"
#include "lamport.h"

struct Menel_status
{
    int id;
    int weight;
};

class Staff : public Communicator, public Lamport
{
public:
    Staff();
    Staff(int positions_number, MPI_Comm COMM_WORLD, MPI_Comm COMM_STAFF);
    void participate();

private:
    int management_id;
    int world_group_size;
    int staff_group_size;
    int positions_number;
    vector<int> staff;
    vector<Menel_status> drunk_menels;
    MPI_Comm COMM_STAFF, COMM_WORLD;
    Packet p;

    void read_by_tag(int tag);
    int get_random(int lower, int upper);
    void get_menels_out();
};

#endif // STAFF_H
