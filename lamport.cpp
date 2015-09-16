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

void Lamport::add(int id)
{
    Member m;
    m.id = id;
    m.timestamp = 0;
    priority_list.push_back(m);
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

bool lamport_sort(const Member &first, const Member &second)
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

bool id_sort(const Member &first, const Member &second)
{
  if (first.timestamp == CONSTANT::NOT_PARTICIPATING)
      return false;
  else if (second.timestamp == CONSTANT::NOT_PARTICIPATING)
      return true;
  else
      return first.id < second.id;

}

void Lamport::sort_list()
{
    sort (this->priority_list.begin(), this->priority_list.end(), lamport_sort);
}

void Lamport::sort_list_by_id()
{
  sort (this->priority_list.begin(), this->priority_list.end(), id_sort);
}

void Lamport::print_list()
{
    for (unsigned int i=0; i<priority_list.size(); i++)
        cout << "Proccess number " << priority_list[i].id << ", timestamp " << priority_list[i].timestamp <<", position " << i << endl;
}

int Lamport::get_my_position()
{
    for (unsigned int i=0; i<priority_list.size(); i++)
        if (get_group_id() == priority_list[i].id) return i+1;

    return -1;
}

int Lamport::get_id_by_position(unsigned int position)
{
    return this->priority_list[position].id;
}

int Lamport::get_timestamp_by_id(unsigned int id)
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
            if (i >= n)
                return true;
            else
                return false;
        }
    return true;
}

int Lamport::active_participants_count()
{
  for (unsigned int i=0; i<priority_list.size(); i++)
      if (priority_list[i].timestamp == CONSTANT::NOT_PARTICIPATING)
      {
        return i;
      }
      return priority_list.size();
}

int Lamport::size()
{
  return priority_list.size();
}

void Lamport::clear(){
  priority_list.clear();
}
