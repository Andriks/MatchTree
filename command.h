#ifndef COMMAND_H
#define COMMAND_H

#include "cell.h"
#include "tile.h"

#include <QSharedPointer>

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
    SwapCommand(QSharedPointer<Tile> from, QSharedPointer<Tile> to);
    SwapCommand(const Cell &from, const Cell &to);

    virtual void exec();
    virtual bool animated();

    QSharedPointer<Tile> from() const;
    void setFrom(QSharedPointer<Tile> from);

    QSharedPointer<Tile> to() const;
    void setTo(QSharedPointer<Tile> to);

private:
    QSharedPointer<Tile> from_;
    QSharedPointer<Tile> to_;
};

//////////////////////////////////////////////////////////////
class MoveUpCommand: public Command
{
public:
    MoveUpCommand(QSharedPointer<Tile> target);

    virtual void exec();
    virtual bool animated();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);

private:
    QSharedPointer<Tile> target_;
};


//////////////////////////////////////////////////////////////
class OpacityCommand: public Command
{
public:
    OpacityCommand(QSharedPointer<Tile> target, float opacity);

    virtual void exec();
    virtual bool animated();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);

    float opacity() const;
    void setOpacity(float opacity);

private:
    QSharedPointer<Tile> target_;
    float opacity_;

};

//////////////////////////////////////////////////////////////
class CreateCommand: public Command
{
public:
    CreateCommand(QSharedPointer<Tile> target);

    virtual void exec();
    virtual bool animated();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);

private:
    QSharedPointer<Tile> target_;
};

//////////////////////////////////////////////////////////////
class RefreshCommand: public Command
{
public:
    RefreshCommand(QSharedPointer<Tile> target);

    virtual void exec();
    virtual bool animated();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);

private:
    QSharedPointer<Tile> target_;
};


#endif // COMMAND_H
