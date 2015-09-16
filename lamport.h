#ifndef LAMPORT_H
#define LAMPORT_H

#include "actor.h"

struct Member
{
public:
    unsigned int id;
    int timestamp;

};

class Lamport : public virtual Actor
{
public:
    Lamport();
    void init_priority_list(vector<int> list);
    void add(int id);
    void update_other_timestamp(unsigned int id, int timestamp);
    void update_my_timestamp(int timestamp);
    void sort_list();
    void sort_list_by_id();
    void print_list();
    int get_my_position();
    int get_id_by_position(unsigned int position);
    int get_timestamp_by_id(unsigned int id);
    bool enough_participants(unsigned int n);
    int active_participants_count();
    int size();
    void clear();

private:
    vector<Member> priority_list;
};

#endif // LAMPORT_H
