#include "package.h"
#include "tilesmodel.h"


Package::Package()
{

}

void Package::push(QSharedPointer<Command> comm)
{
    m_commList.enqueue(comm);
}

void Package::exec()
{
    bool animated = true;

    while (m_commList.size()) {
        QSharedPointer<Command> comm = m_commList.dequeue();
        animated = comm->animated();

        comm->exec();
    }

    if (!animated)
        TilesModel::Instance()->execNextPackage();
}

void Package::clear()
{
    while (m_commList.size()) {
        m_commList.dequeue();
    }
}

size_t Package::size()
{
    return m_commList.size();
}

