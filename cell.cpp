#include "cell.h"
#include "tilesmodel.h"

#include <cmath>


//int Cell::width_(0);
//int Cell::height_(0);


Cell::Cell() :
    row(0),
    col(0)
{

}

Cell::Cell(const Cell &cell):
    row(cell.row),
    col(cell.col)
{

}

Cell::Cell(int r, int c) :
    row(r),
    col(c)
{

}

Cell::Cell(int _index)
{
    row = std::ceil(_index / TilesModel::Instance()->width()) + 1;
    col = (_index + 1) - ((row - 1) * TilesModel::Instance()->width());
}

bool Cell::operator ==(const Cell cell)
{
    if ((row == cell.row) && (col == cell.col))
        return true;

    return false;
}

bool Cell::valid() const
{
    if ((row <= 0) || (row > TilesModel::Instance()->height()))
        return false;

    if ((col <= 0) || (col > TilesModel::Instance()->width()))
        return false;

    return true;
}

int Cell::index() const
{
    return ((row - 1) * TilesModel::Instance()->width()) + col - 1;
}

//void Cell::setStaticParams(int width, int height)
//{
//    width_ = width;
//    height_ = height;
//}
