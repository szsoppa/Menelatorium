#include "staff.h"

Staff::Staff()
{
}

Staff::Staff(int positions_number, MPI_Comm COMM_WORLD, MPI_Comm COMM_STAFF)
{
    this->positions_number = positions_number;
    this->COMM_WORLD = COMM_WORLD;
    this->COMM_STAFF = COMM_STAFF;
    MPI_Comm_size(this->COMM_STAFF, &staff_group_size);
    MPI_Comm_size(this->COMM_WORLD, &world_group_size);
    management_id = world_group_size - 1;
    for (int i=0; i<this->staff_group_size; i++) this->staff.push_back(i);
}

void Staff::participate()
{
    MPI_Status message_status;
    init_priority_list(staff);
    read(p.get(), p.get_size(), management_id, MANAGEMENT::OPEN_EXHIBITION, COMM_WORLD);
    for (int i=0; i<positions_number; i++)
    {
        read(p.get(), p.get_size(), MPI_ANY_SOURCE, MPI_ANY_TAG, COMM_WORLD, &message_status);
        if (message_status.MPI_TAG == MENEL::DRUNK)
        {
            Menel_status temp;
            temp.id = message_status.MPI_SOURCE;
            temp.weight = p.get_message();
            drunk_menels.push_back(temp);
        }
    }
    get_menels_out();
}

void Staff::read_by_tag(int tag)
{
    MPI_Status message_status;
    switch(tag)
    {
        case STAFF::TIMESTAMP:
        {
            for (int i=0; i<this->staff_group_size; i++)
            {
                if (get_group_id() == i) continue;
                read(p.get(), p.get_size(), i, STAFF::TIMESTAMP, COMM_STAFF);
                update_other_timestamp(i, p.get_timestamp());
            }
            break;
        }
        case STAFF::BACK_FROM_MENEL:
        {
            read(p.get(), p.get_size(), i, STAFF::BACK_FROM_MENEL, COMM_STAFF);
        }
        default:
            break;
    }
}

int Staff::get_random(int lower, int upper)
{
    std::random_device rd;
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int> uniform_dist(lower, upper);
    int mean = uniform_dist(e1);
    return mean;
}

void Staff::get_menels_out()
{
    broadcast(p.get(), p.get_size(), STAFF::TIMESTAMP , this->COMM_MENELS);
    read_by_tag(STAFF::TIMESTAMP);
    for (unsigned int i=0; i<drunk_menels.size(); i++)
    {
        sort_list();
        if (get_my_position() <= drunk_menels[i].weight && enough_participants(drunk_menels[i].weight))
        {

        }
        else if (get_my_position() <= drunk_menels[i].weight && !enough_participants(drunk_menels[i].weight))
        {

        }
        else
        {
            for(int j=0; j<drunk_menels[i].weight; j++)
                update_other_timestamp(get_id_by_position(j), CONSTANT::NOT_PARTICIPATING);
        }
    }
    drunk_menels.clear();
}
