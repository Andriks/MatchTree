#ifndef COMMAND_H
#define COMMAND_H

#include "cell.h"
#include "tile.h"

class Command
{
public:
    Command();

    virtual void exec() = 0;
    virtual bool animated() = 0;

};

//////////////////////////////////////////////////////////////
class SwapCommand: public Command
{
public:
    SwapCommand(Tile *from, Tile *to);
    SwapCommand(const Cell &from, const Cell &to);

    virtual void exec();
    virtual bool animated();

    Tile *from() const;
    void setFrom(Tile *from);

    Tile *to() const;
    void setTo(Tile *to);

private:
    Tile *from_;
    Tile *to_;
};

//////////////////////////////////////////////////////////////
class MoveUpCommand: public Command
{
public:
    MoveUpCommand(Tile *target);

    virtual void exec();
    virtual bool animated();

    Tile *target() const;
    void setTarget(Tile *target);

private:
    Tile *target_;
};

//////////////////////////////////////////////////////////////
class MoveDownCommand: public Command
{
public:
    MoveDownCommand(Tile *target);

    virtual void exec();
    virtual bool animated();

    Tile *target() const;
    void setTarget(Tile *target);

private:
    Tile *target_;
};

//////////////////////////////////////////////////////////////
class OpacityCommand: public Command
{
public:
    OpacityCommand(Tile *target, float opacity);

    virtual void exec();
    virtual bool animated();

    Tile *target() const;
    void setTarget(Tile *target);

    float opacity() const;
    void setOpacity(float opacity);

private:
    Tile *target_;
    float opacity_;

};

//////////////////////////////////////////////////////////////
class CreateCommand: public Command
{
public:
    CreateCommand(Tile *target);

    virtual void exec();
    virtual bool animated();

    Tile *target() const;
    void setTarget(Tile *target);

private:
    Tile *target_;
};


#endif // COMMAND_H
