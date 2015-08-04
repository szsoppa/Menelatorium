#include "menel.h"

Menel::Menel()
{
}

Menel::Menel(int positions_number, MPI_Comm COMM_WORLD, MPI_Comm COMM_MENELS)
{
    weight = get_random(1,4);
    this->positions_number = positions_number;
    this->COMM_WORLD = COMM_WORLD;
    this->COMM_MENELS = COMM_MENELS;
    MPI_Comm_size(this->COMM_MENELS, &menel_group_size);
    MPI_Comm_size(this->COMM_WORLD, &world_group_size);
    management_id = world_group_size - 1;
    for (int i=0; i<this->menel_group_size; i++) this->menels.push_back(i);
}

void Menel::participate()
{
    int counter = -1;
    bool participate = false;
    int menel_timestamp = 0;
    init_priority_list(menels);
    while (true)
    {
        counter++;
        if (get_random(1,50) > 45)
            participate = false;
        else
            participate = true;

        if (participate)
            menel_timestamp = get_timestamp();
        else
            menel_timestamp = CONSTANT::NOT_PARTICIPATING;

        update_other_timestamp(get_group_id(), menel_timestamp);
        p.set(menel_timestamp, CONSTANT::NOT_IMPORTANT);
        broadcast(p.get(), p.get_size(), MENEL::TIMESTAMP , this->COMM_MENELS);
        read_by_tag(MENEL::TIMESTAMP);
        sort_list();
        if (!enough_participants(positions_number)) continue;
        if (participate && get_my_position() <= positions_number)
        {
            cout << "Proces " << get_group_id() << ", iteracja "<<counter<<", wszedlem do sekcji"<<endl;
            if (get_my_position() == positions_number)
            {
                p.set(get_timestamp(), 1);
                send(p.get(), p.get_size(), management_id, MANAGEMENT::LAST_MENEL_IN, COMM_WORLD);
                cout << "-"<<"Proces nr "<<get_group_id()<<", iteracja: "<<counter<<", wyslalem do procesu " << management_id << "-" << endl;
            }
            enter_exhibition();
        }
        else
        {
            read(p.get(), p.get_size(), management_id, MANAGEMENT::OPEN_EXHIBITION, COMM_WORLD);
        }
        read(p.get(), p.get_size(), management_id, MANAGEMENT::END_OF_EXHIBITION, COMM_WORLD);
    }
}

void Menel::read_by_tag(int tag)
{
    MPI_Status message_status;
    switch(tag)
    {
        case MENEL::TIMESTAMP:
        {
            for (int i=0; i<this->menel_group_size; i++)
            {
                if (get_group_id() == i) continue;
                read(p.get(), p.get_size(), i, MENEL::TIMESTAMP, COMM_MENELS, &message_status);
                update_other_timestamp(message_status.MPI_SOURCE, p.get_timestamp());
            }
            break;
        }
        case STAFF::HELP:
        {
            for (int i=0; i<this->weight; i++)
            {
                read(p.get(), p.get_size(), MPI_ANY_SOURCE, STAFF::HELP, COMM_WORLD);
            }
            break;
        }
    }
}

int Menel::get_positions_number()
{
    return this->positions_number;
}

void Menel::enter_exhibition()
{
    read(p.get(), p.get_size(), management_id, MANAGEMENT::OPEN_EXHIBITION, COMM_WORLD);

    drink_some_vodka();
    vomit();
    say_some_dayum();

    if (get_random(0, 1))
    {
        broadcast(p.get(), p.get_size(), MENEL::DRUNK, COMM_WORLD);
        read_by_tag(STAFF::HELP);
    }
}

int Menel::get_random(int lower, int upper)
{
    std::random_device rd;
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int> uniform_dist(lower, upper);
    int mean = uniform_dist(e1);
    return mean;
}

void Menel::drink_some_vodka()
{
}

void Menel::vomit()
{
}

void Menel::say_some_dayum()
{
}
