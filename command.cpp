#include "command.h"
#include "tilesmodel.h"

#include "QDebug"

Command::Command()
{

}


//////////////////////////////////////////////////////////////
SwapCommand::SwapCommand(Tile *from, Tile *to) :
    from_(from),
    to_(to)
{

}

SwapCommand::SwapCommand(const Cell &from, const Cell &to) :
    from_(NULL),
    to_(NULL)
{
    from_ = TilesModel::Instance()->item(from.index());
    to_ = TilesModel::Instance()->item(to.index());
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

    int swap_cnt = 0;

    while (cell.upper().valid()) {
        TilesModel::Instance()->swapCells(cell, cell.upper());

        cell = cell.upper();
        swap_cnt++;
    }

    if (swap_cnt == 0)
        TilesModel::Instance()->execNextPackage();
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
MoveDownCommand::MoveDownCommand(Tile *target) :
    target_(target)
{

}

void MoveDownCommand::exec()
{
    Cell cell = Cell(target_->index());

    int swap_cnt = 0;

    Tile *lower_tile = TilesModel::Instance()->item(cell.lower().index());

    while (!lower_tile->valid()) {
        TilesModel::Instance()->swapCells(cell, cell.lower());

        cell = cell.lower();
        lower_tile = TilesModel::Instance()->item(cell.lower().index());

        swap_cnt++;
    }

    if (swap_cnt == 0)
        TilesModel::Instance()->execNextPackage();
}
Tile *MoveDownCommand::target() const
{
    return target_;
}

void MoveDownCommand::setTarget(Tile *target)
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
    target_->setValid(false);
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


