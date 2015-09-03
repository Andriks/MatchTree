#include "package.h"
#include "tilesmodel.h"

#include <cmath>

#include <QDebug>


Package::Package() :
    m_duration(0)
{

}

void Package::push(QSharedPointer<Command> comm) {
    m_commList.enqueue(comm);
}

void Package::exec() {
    while (m_commList.size()) {
        QSharedPointer<Command> comm = m_commList.dequeue();
        m_duration = std::max(m_duration, comm->duration());

        comm->exec();
    }
}

void Package::clear() {
    while (m_commList.size()) {
        m_commList.dequeue();
    }
}

size_t Package::size() {
    return m_commList.size();
}
int Package::duration() const {
    return m_duration;
    qDebug() << m_duration;
}

//void Package::setDuration(int delay) {
//    m_duration = delay;
//}


