#include "command.h"
#include "tilesmodel.h"

Command::Command()
{

}


MoveCommand::MoveCommand(Cell from, Cell to) :
    from_(from),
    to_(to)
{

}

//////////////////////////////////////////////////////////////
void MoveCommand::exec()
{
    TilesModel::Instance()->swapCells(from_, to_);
}

Cell MoveCommand::from() const
{
    return from_;
}

void MoveCommand::setFrom(const Cell &from)
{
    from_ = from;
}
Cell MoveCommand::to() const
{
    return to_;
}

void MoveCommand::setTo(const Cell &to)
{
    to_ = to;
}


//////////////////////////////////////////////////////////////
OpacityCommand::OpacityCommand(Cell target, float opacity) :
    target_(target),
    opacity_(opacity)
{

}

void OpacityCommand::exec()
{
    TilesModel::Instance()->changeOpacity(target_, opacity_);
}

Cell OpacityCommand::target() const
{
    return target_;
}

void OpacityCommand::setTarget(const Cell &target)
{
    target_ = target;
}
float OpacityCommand::opacity() const
{
    return opacity_;
}

void OpacityCommand::setOpacity(float opacity)
{
    opacity_ = opacity;
}


