#include "package.h"
#include "tilesmodel.h"


Package::Package()
{

}

void Package::push(QSharedPointer<Command> comm)
{
    comm_list_.enqueue(comm);
}

void Package::exec()
{
    bool animated = true;

    while (comm_list_.size()) {
        QSharedPointer<Command> comm = comm_list_.dequeue();
        animated = comm->animated();

        comm->exec();
    }

    if (!animated)
        TilesModel::Instance()->execNextPackage();
}

void Package::clear()
{
    while (comm_list_.size()) {
        comm_list_.dequeue();
    }
}

size_t Package::size()
{
    return comm_list_.size();
}

