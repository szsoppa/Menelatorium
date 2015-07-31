#ifndef LAMPORT_H
#define LAMPORT_H

#include "actor.h"

struct Member
{
public:
    unsigned int id;
    int timestamp;
    bool operator() (const Member &first, const Member &second)
    {
        if (first.timestamp == CONSTANT::NOT_PARTICIPATING)
            return false;
        else if (second.timestamp == CONSTANT::NOT_PARTICIPATING)
            return true;
        else if (first.timestamp == second.timestamp)
            return first.id < second.id;
        else
            return first.timestamp < second.timestamp;
    }
};

class Lamport : public virtual Actor
{
public:
    Lamport();
    void init_priority_list(vector<int> list);
    void update_other_timestamp(unsigned int id, int timestamp);
    void update_my_timestamp(int timestamp);
    void sort_list();
    void print_list();
    unsigned int get_my_position();
    unsigned int get_id_by_position(unsigned int position);
    unsigned int get_timestamp_by_id(unsigned int id);
    bool enough_participants(unsigned int n);
    
private:
    vector<Member> priority_list;
};

#endif // LAMPORT_H
