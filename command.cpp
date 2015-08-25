#include "command.h"
#include "tilesmodel.h"

Command::Command()
{

}


MoveCommand::MoveCommand(Tile *from, Tile *to) :
    from_(from),
    to_(to)
{

}

//////////////////////////////////////////////////////////////
void MoveCommand::exec()
{
        TilesModel::Instance()->swapCells(from_->getIndex(), to_->getIndex());
}

Tile *MoveCommand::from() const
{
    return from_;
}

void MoveCommand::setFrom(Tile *from)
{
    from_ = from;
}
Tile *MoveCommand::to() const
{
    return to_;
}

void MoveCommand::setTo(Tile *to)
{
    to_ = to;
}


//////////////////////////////////////////////////////////////
MoveUpCommand::MoveUpCommand(Tile *target) :
    target_(target)
{

}

void MoveUpCommand::exec()
{
    Cell cell = Cell(target_->getIndex());
//    Cell upper_cell = cell.upper();

    while (cell.upper().valid()) {
        TilesModel::Instance()->swapCells(cell, cell.upper());

        cell = cell.upper();
    }
}

Tile *MoveUpCommand::target() const
{
    return target_;
}

void MoveUpCommand::setTarget(Tile *target)
{
    target_ = target;
}


//////////////////////////////////////////////////////////////
OpacityCommand::OpacityCommand(Tile *target, float opacity) :
    target_(target),
    opacity_(opacity)
{

}

void OpacityCommand::exec()
{
    TilesModel::Instance()->changeOpacity(target_, opacity_);
}

Tile *OpacityCommand::target() const
{
    return target_;
}

void OpacityCommand::setTarget(Tile *target)
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



//////////////////////////////////////////////////////////////
CreateCommand::CreateCommand(Tile *target) :
    target_(target)
{

}

void CreateCommand::exec()
{

}

Tile *CreateCommand::target() const
{
    return target_;
}

void CreateCommand::setTarget(Tile *target)
{
    target_ = target;
}

