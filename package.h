#ifndef PACKAGE_H
#define PACKAGE_H

#include "command.h"

#include <queue>

class Package
{
public:
    Package();

    void push(Command *comm);
    void exec();
    void clear();

private:
    std::queue<Command *> comm_list_;

};

#endif // PACKAGE_H
