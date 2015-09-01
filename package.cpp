#include "package.h"
#include "tilesmodel.h"

#include <cmath>


Package::Package() :
    m_delay(0)
{

}

void Package::push(QSharedPointer<Command> comm) {
    m_commList.enqueue(comm);
}

void Package::exec() {
    while (m_commList.size()) {
        QSharedPointer<Command> comm = m_commList.dequeue();
        m_delay = std::max(m_delay, comm->delay());

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
int Package::delay() const {
    return m_delay;
}

void Package::setDelay(int delay) {
    m_delay = delay;
}


