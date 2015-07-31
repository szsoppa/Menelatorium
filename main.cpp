#include "menel.h"

MPI_Comm COMM_MENELS, COMM_STAFF;
MPI_Group GROUP_WORLD, GROUP_MENELS, GROUP_STAFF;

int menels_number, positions_number, staff_number, size;

int *prepare_group_list(int *temp, int n);
int *get_menels_list();
int *get_staff_list();

int main(int argc, char** argv)
{
    int type, id;
    istringstream temp1(argv[1]);
    temp1 >> staff_number;
    istringstream temp2(argv[2]);
    temp2 >> positions_number;
    istringstream temp3(argv[3]);
    temp3 >> menels_number;

    if (argc < 4)
    {
        cout << "You provided to few arguments." << endl <<"There should be " << 4 - argc << " more argument(s)." << endl;
        exit (EXIT_FAILURE);
    }

    if ((4*staff_number >= positions_number) || (menels_number <= positions_number))
    {
        cout << "Wrong number of staff or menels." << endl;
        exit (EXIT_FAILURE);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if ((staff_number + menels_number + 1) != size)
    {
        cout << "Too much or too less processes initialized." << endl;
        MPI_Finalize();
        exit (EXIT_FAILURE);
    }

    else if (id >= 0 && id < menels_number)
        type = ACTORS::MENEL;
    else if (id >= menels_number && id < (menels_number + staff_number))
        type = ACTORS::STAFF;
    else
        type = ACTORS::MANAGEMENT;

    int *list_without_menels = prepare_group_list(get_menels_list(), menels_number);
    int *list_without_staff = prepare_group_list(get_staff_list(), staff_number);

    MPI_Comm_group(MPI_COMM_WORLD, &GROUP_WORLD);
    MPI_Group_excl(GROUP_WORLD, (size-menels_number), list_without_menels, &GROUP_MENELS);
    MPI_Comm_create(MPI_COMM_WORLD, GROUP_MENELS, &COMM_MENELS);
    MPI_Group_excl(GROUP_WORLD, (size-staff_number), list_without_staff, &GROUP_STAFF);
    MPI_Comm_create(MPI_COMM_WORLD, GROUP_STAFF, &COMM_STAFF);

    switch (type)
    {
        case ACTORS::MENEL:
        {
            int menel_id;
            MPI_Comm_rank(COMM_MENELS, &menel_id);
            Menel m(positions_number, MPI_COMM_WORLD, COMM_MENELS);
            m.set_world_id(id);
            m.set_group_id(menel_id);
            m.participate();
        }
        case ACTORS::STAFF:
        {

        }
        case ACTORS::MANAGEMENT:
        {

        }
        default:
            break;
    }
    MPI_Finalize();
}

int *prepare_group_list(int *temp, int n)
{
    bool flag = false;
    int counter = 0;
    int *tab = new int[size - n];

    for (int i=0; i<size; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(temp[j] == i)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            tab[counter++] = i;
        flag = false;
    }
    return tab;
}

int *get_menels_list()
{
    int *tab = new int[menels_number];
    for (int i=0; i<menels_number; i++)
        tab[i] = i;
    return tab;
}

int *get_staff_list()
{
    int *tab = new int[staff_number];
    for (int i=0; i<staff_number; i++)
        tab[i] = i+menels_number;
    return tab;
}
