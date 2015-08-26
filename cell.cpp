#include "cell.h"
#include "tilesmodel.h"

#include <cmath>


Cell::Cell() :
    row_(0),
    col_(0)
{

}

Cell::Cell(const Cell &cell):
    row_(cell.row_),
    col_(cell.col_)
{

}

Cell::Cell(int row, int column) :
    row_(row),
    col_(column)
{

}

Cell::Cell(int index)
{
    row_ = std::ceil(index / TilesModel::Instance()->width()) + 1;
    col_ = (index + 1) - ((row_ - 1) * TilesModel::Instance()->width());
}


int Cell::row() const
{
    return row_;
}

void Cell::setRow(int row)
{
    row_ = row;
}

int Cell::col() const
{
    return col_;
}

void Cell::setCol(int col)
{
    col_ = col;
}

bool Cell::operator ==(const Cell cell)
{
    if ((row_ == cell.row_) && (col_ == cell.col_))
        return true;

    return false;
}

bool Cell::valid() const
{
    if ((row_ <= 0) || (row_ > TilesModel::Instance()->height()))
        return false;

    if ((col_ <= 0) || (col_ > TilesModel::Instance()->width()))
        return false;

    return true;
}

int Cell::index() const
{
    return ((row_ - 1) * TilesModel::Instance()->width()) + col_ - 1;
}

Cell Cell::upper()
{
    return Cell(row_ - 1, col_);
}

Cell Cell::lower()
{
    return Cell(row_ + 1, col_);
}

Cell Cell::right()
{
    return Cell(row_, col_ + 1);
}

Cell Cell::left()
{
    return Cell(row_, col_ - 1);
}

