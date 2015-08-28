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

bool SwapCommand::animated()
{
    return true;
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

    while (cell.upper().valid()) {
        TilesModel::Instance()->swapCells(cell, cell.upper());

        cell = cell.upper();
    }
}

bool MoveUpCommand::animated()
{
    return true;
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
    Cell curr_cell = Cell(target_->index());
    Tile *lower_tile = TilesModel::Instance()->item(curr_cell.lower().index());

    while (!lower_tile->valid()) {
        TilesModel::Instance()->swapCells(curr_cell, curr_cell.lower());

        curr_cell = curr_cell.lower();
        lower_tile = TilesModel::Instance()->item(curr_cell.lower().index());
    }
}

bool MoveDownCommand::animated()
{
    return true;
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

bool OpacityCommand::animated()
{
    return true;
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
    TilesModel::Instance()->createItem(target_->index());
}

bool CreateCommand::animated()
{
    return false;
}

Tile *CreateCommand::target() const
{
    return target_;
}

void CreateCommand::setTarget(Tile *target)
{
    target_ = target;
}

//////////////////////////////////////////////////////////////
RefreshCommand::RefreshCommand(Tile *target) :
    target_(target)
{

}

void RefreshCommand::exec()
{
    TilesModel::Instance()->refreshItem(target_->index());
}

bool RefreshCommand::animated()
{
    return false;
}

Tile *RefreshCommand::target() const
{
    return target_;
}

void RefreshCommand::setTarget(Tile *target)
{
    target_ = target;
}
