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
    virtual int duration() = 0;

//    virtual ~Command();

};

//////////////////////////////////////////////////////////////
class SwapCommand: public Command
{
public:
    SwapCommand(QSharedPointer<Tile> from, QSharedPointer<Tile> to);
    SwapCommand(const Cell &from, const Cell &to);

    virtual void exec();
    virtual int duration();

    QSharedPointer<Tile> from() const;
    void setFrom(QSharedPointer<Tile> from);

    QSharedPointer<Tile> to() const;
    void setTo(QSharedPointer<Tile> to);

private:
    QSharedPointer<Tile> m_from;
    QSharedPointer<Tile> m_to;
};

//////////////////////////////////////////////////////////////
class MoveUpCommand: public Command
{
public:
    MoveUpCommand(QSharedPointer<Tile> target);

    virtual void exec();
    virtual int duration();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);

private:
    QSharedPointer<Tile> m_target;
};


//////////////////////////////////////////////////////////////
class OpacityCommand: public Command
{
public:
    OpacityCommand(QSharedPointer<Tile> target, float opacity);

    virtual void exec();
    virtual int duration();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);

    float opacity() const;
    void setOpacity(float opacity);

private:
    QSharedPointer<Tile> m_target;
    float m_opacity;

};

//////////////////////////////////////////////////////////////
class ScaleCommand: public Command
{
public:
    ScaleCommand(QSharedPointer<Tile> target, float scale);

    virtual void exec();
    virtual int duration();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);


    float scale() const;
    void setScale(float scale);

private:
    QSharedPointer<Tile> m_target;
    float m_scale;

};

//////////////////////////////////////////////////////////////
class CreateCommand: public Command
{
public:
    CreateCommand(QSharedPointer<Tile> target);

    virtual void exec();
    virtual int duration();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);

private:
    QSharedPointer<Tile> m_target;
};

//////////////////////////////////////////////////////////////
class RefreshCommand: public Command
{
public:
    RefreshCommand(QSharedPointer<Tile> target);

    virtual void exec();
    virtual int duration();

    QSharedPointer<Tile> target() const;
    void setTarget(QSharedPointer<Tile> target);

private:
    QSharedPointer<Tile> m_target;
};


#endif // COMMAND_H
