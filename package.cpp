#include "package.h"
#include "tilesmodel.h"


Package::Package()
{

}

void Package::push(Command *comm)
{
    comm_list_.push(comm);
}

void Package::exec()
{
    bool animated;

    while (comm_list_.size()) {
        Command *comm = comm_list_.front();
        comm_list_.pop();
        animated = comm->animated();

        comm->exec();
        delete comm;
    }

    if (!animated)
        TilesModel::Instance()->execNextPackage();
}

void Package::clear()
{
    while (comm_list_.size()) {
        comm_list_.pop();
    }
}

