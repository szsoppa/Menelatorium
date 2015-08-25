#include "management.h"

Management::Management(int positions_number, MPI_Comm COMM_WORLD)
{
    this->positions_number = positions_number;
    this->COMM_WORLD = COMM_WORLD;
    MPI_Comm_size(this->COMM_WORLD, &world_group_size);
}

void Management::participate()
{
    MPI_Status mess_status;
    bool active = true;
    while(active)
    {
        cout << "wbijam" << endl;
        read(p.get(), p.get_size(), MPI_ANY_SOURCE, MANAGEMENT::LAST_MENEL_IN, COMM_WORLD, &mess_status);
        cout << "Odebralem" << endl;
        p.set(get_timestamp(), 1);
        broadcast(p.get(), p.get_size(), MANAGEMENT::OPEN_EXHIBITION, COMM_WORLD);
        broadcast(p.get(), p.get_size(), MANAGEMENT::END_OF_EXHIBITION, COMM_WORLD);
    }
}
