#include "lamport.h"

Lamport::Lamport()
{
}

void Lamport::init_priority_list(vector<int> list)
{
    for (unsigned int i=0; i<list.size(); i++)
    {
        Member m;
        m.id = list[i];
        m.timestamp = 0;
        priority_list.push_back(m);
    }
}

void Lamport::update_other_timestamp(unsigned int id, int timestamp)
{
    for(unsigned int i=0; i<priority_list.size(); i++)
        if(priority_list[i].id == id) priority_list[i].timestamp = timestamp;
}

void Lamport::update_my_timestamp(int timestamp)
{
    set_timestamp(timestamp);
}

void Lamport::sort_list()
{
    sort (this->priority_list.begin(), this->priority_list.end(), Member());
}

void Lamport::print_list()
{
    for (unsigned int i=0; i<priority_list.size(); i++)
        cout << "Proccess number " << priority_list[i].id << ", timestamp " << priority_list[i].timestamp <<", position " << i << endl;
}

unsigned int Lamport::get_my_position()
{
    for (unsigned int i=0; i<priority_list.size(); i++)
        if (get_group_id() == priority_list[i].id) return i+1;

    return -1;
}

unsigned int Lamport::get_id_by_position(unsigned int position)
{
    return this->priority_list[position].id;
}

unsigned int Lamport::get_timestamp_by_id(unsigned int id)
{
    for (unsigned int i=0; i<priority_list.size(); i++)
        if(priority_list[i].id == id) return priority_list[i].timestamp;

    return -1;
}

bool Lamport::enough_participants(unsigned int n)
{
    for (unsigned int i=0; i<priority_list.size(); i++)
        if (priority_list[i].timestamp == CONSTANT::NOT_PARTICIPATING)
        {
            if (i+1 > n)
                return true;
            else
                return false;
        }
    return true;
}

