#ifndef PACKAGE_H
#define PACKAGE_H

#include "command.h"

#include <QQueue>
#include <QSharedPointer>

class Package
{
public:
    Package();

    void push(QSharedPointer<Command> comm);
    void exec();
    void clear();
    size_t size();

private:
    QQueue<QSharedPointer<Command> > m_commList;

};

#endif // PACKAGE_H
