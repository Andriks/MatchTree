#include "package.h"


Package::Package()
{

}

void Package::push(Command *comm)
{
    comm_list_.push(comm);
}

void Package::exec()
{
    while (comm_list_.size()) {
        Command *comm = comm_list_.front();
        comm_list_.pop();

        comm->exec();
        delete comm;
    }
}

void Package::clear()
{
    while (comm_list_.size()) {
        comm_list_.pop();
    }
}

