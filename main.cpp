#include "menel.h"

int main()
{
    int temp = 10;
    Menel m;
    vector<int> l;
    for (int i=0; i<10; i++)
    {
        l.push_back(i);
    }
    m.init_priority_list(l);
    for (int i=0; i<10; i++)
    {
        if (i%2 == 0) m.update_other_timestamp(i,-100);
        else m.update_other_timestamp(i,temp);
        temp--;
    }
    m.sort_list();
    m.print_list();
}

