#ifndef COMMAND_H
#define COMMAND_H

#include "cell.h"

class Command
{
public:
    Command();

    virtual void exec() = 0;

};

//////////////////////////////////////////////////////////////
class MoveCommand: public Command
{
public:
    MoveCommand(Cell from, Cell to);

    virtual void exec();

    Cell from() const;
    void setFrom(const Cell &from);

    Cell to() const;
    void setTo(const Cell &to);

private:
    Cell from_;
    Cell to_;
};

//////////////////////////////////////////////////////////////
class OpacityCommand: public Command
{
public:
    OpacityCommand(Cell target, float opacity);

    virtual void exec();

    Cell target() const;
    void setTarget(const Cell &target);

    float opacity() const;
    void setOpacity(float opacity);

private:
    Cell target_;
    float opacity_;

};


#endif // COMMAND_H
