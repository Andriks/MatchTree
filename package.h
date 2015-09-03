#ifndef PACKAGE_H
#define PACKAGE_H

#include "command.h"

#include <QQueue>
#include <QSharedPointer>

class Package
{
public:
    Package();

    void exec();

    void push(QSharedPointer<Command> comm);
    void clear();
    size_t size();

    int duration() const;

private:
    QQueue<QSharedPointer<Command> > m_commList;
    int m_duration;

};

#endif // PACKAGE_H
