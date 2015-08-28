#ifndef PACKAGE_H
#define PACKAGE_H

#include "command.h"

#include <queue>
#include <QSharedPointer>

class Package
{
public:
    Package();

    void push(QSharedPointer<Command> comm);
    void exec();
    void clear();

private:
    std::queue<QSharedPointer<Command> > comm_list_;

};

#endif // PACKAGE_H
