#include "cell.h"
#include "tilesmodel.h"

#include <cmath>


Cell::Cell() :
    m_row(0),
    m_col(0)
{

}

Cell::Cell(const Cell &cell):
    m_row(cell.m_row),
    m_col(cell.m_col)
{

}

Cell::Cell(int row, int column) :
    m_row(row),
    m_col(column)
{

}

Cell::Cell(int index)
{
    m_row = std::ceil(index / TilesModel::Instance()->width()) + 1;
    m_col = (index + 1) - ((m_row - 1) * TilesModel::Instance()->width());
}


int Cell::row() const
{
    return m_row;
}

void Cell::setRow(int row)
{
    m_row = row;
}

int Cell::col() const
{
    return m_col;
}

void Cell::setCol(int col)
{
    m_col = col;
}

bool Cell::operator ==(const Cell cell)
{
    if ((m_row == cell.m_row) && (m_col == cell.m_col))
        return true;

    return false;
}

bool Cell::valid() const
{
    if ((m_row <= 0) || (m_row > (TilesModel::Instance()->height() + 1)))
        return false;

    if ((m_col <= 0) || (m_col > TilesModel::Instance()->width()))
        return false;

    return true;
}

int Cell::index() const
{
    return ((m_row - 1) * TilesModel::Instance()->width()) + m_col - 1;
}

Cell Cell::upper()
{
    return Cell(m_row + 1, m_col);
}

Cell Cell::lower()
{
    return Cell(m_row - 1, m_col);
}

Cell Cell::right()
{
    return Cell(m_row, m_col + 1);
}

Cell Cell::left()
{
    return Cell(m_row, m_col - 1);
}

