#include "command.h"
#include "tilesmodel.h"

#include "QDebug"

Command::Command()
{

}


//////////////////////////////////////////////////////////////
SwapCommand::SwapCommand(QSharedPointer<Tile> from, QSharedPointer<Tile> to) :
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

QSharedPointer<Tile> SwapCommand::from() const
{
    return from_;
}

void SwapCommand::setFrom(QSharedPointer<Tile> from)
{
    from_ = from;
}

QSharedPointer<Tile> SwapCommand::to() const
{
    return to_;
}

void SwapCommand::setTo(QSharedPointer<Tile> to)
{
    to_ = to;
}


//////////////////////////////////////////////////////////////
MoveUpCommand::MoveUpCommand(QSharedPointer<Tile> target) :
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

QSharedPointer<Tile> MoveUpCommand::target() const
{
    return target_;
}

void MoveUpCommand::setTarget(QSharedPointer<Tile> target)
{
    target_ = target;
}


//////////////////////////////////////////////////////////////
OpacityCommand::OpacityCommand(QSharedPointer<Tile> target, float opacity) :
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

QSharedPointer<Tile> OpacityCommand::target() const
{
    return target_;
}

void OpacityCommand::setTarget(QSharedPointer<Tile> target)
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
CreateCommand::CreateCommand(QSharedPointer<Tile> target) :
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

QSharedPointer<Tile> CreateCommand::target() const
{
    return target_;
}

void CreateCommand::setTarget(QSharedPointer<Tile> target)
{
    target_ = target;
}

//////////////////////////////////////////////////////////////
RefreshCommand::RefreshCommand(QSharedPointer<Tile> target) :
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

QSharedPointer<Tile> RefreshCommand::target() const
{
    return target_;
}

void RefreshCommand::setTarget(QSharedPointer<Tile> target)
{
    target_ = target;
}
