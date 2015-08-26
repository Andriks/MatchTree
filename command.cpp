#include "command.h"
#include "tilesmodel.h"

Command::Command()
{

}


SwapCommand::SwapCommand(Tile *from, Tile *to) :
    from_(from),
    to_(to)
{

}

//////////////////////////////////////////////////////////////
void SwapCommand::exec()
{
    TilesModel::Instance()->swapCells(from_->index(), to_->index());
}

Tile *SwapCommand::from() const
{
    return from_;
}

void SwapCommand::setFrom(Tile *from)
{
    from_ = from;
}
Tile *SwapCommand::to() const
{
    return to_;
}

void SwapCommand::setTo(Tile *to)
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
    Cell cell = Cell(target_->index());

    if (!cell.upper().valid()) {
        // TODO
        return;
    }


    while (cell.upper().valid()) {
        if (TilesModel::Instance()->item(cell.upper().index())->opacity() != 1.0)
            break;

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
    TilesModel::Instance()->createNewItem(target_->index());
}

Tile *CreateCommand::target() const
{
    return target_;
}

void CreateCommand::setTarget(Tile *target)
{
    target_ = target;
}

