#include "communicator.h"

Communicator::Communicator()
{
}

void Communicator::send(int *buff, int count, int destiny , int tag, MPI_Comm comm)
{
    MPI_Request req;
    increment_timestamp();
    MPI_Isend(buff, count, MPI_INT, destiny, tag, comm, &req);
    MPI_Request_free(&req);
}

void Communicator::read(int *buff, int count, int source, int tag, MPI_Comm comm)
{
    MPI_Recv(buff, count, MPI_INT, source, tag, comm, MPI_STATUS_IGNORE);
    set_timestamp( max(buff[0], get_timestamp()) );
    increment_timestamp();
}

void Communicator::read(int *buff, int count, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
    MPI_Recv(buff, count, MPI_INT, source, tag, comm, status);
    set_timestamp( max(buff[0], get_timestamp()) );
    increment_timestamp();
}

void Communicator::broadcast(int *buff, int count, int tag, MPI_Comm comm)
{
    int size,rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
    increment_timestamp();
    for (int i=0; i<size; i++)
    {
        if (i==rank) continue;
        send(buff, count, i, tag, comm);
    }
}

void Communicator::increment_timestamp()
{
    set_timestamp(get_timestamp()+1);
}

void Communicator::save_message(int *buff, int source)
{
    ostringstream ss, ss2;
    ss << get_group_id();
    ofstream myfile;
    myfile.open ("data/"+ss.str()+".txt", ios::app);
    ss << source;
    ss2 << buff[0];
    myfile << "From " << ss.str() << " timestamp: " << ss2.str() << endl;
    myfile.close();
}
