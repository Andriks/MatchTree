#include "tilesmodel.h"

#include <QDebug>

#include <cmath>

int TilesModel::Cell::width_(0);
int TilesModel::Cell::height_(0);

TilesModel::Cell::Cell() :
    row(0),
    col(0)
{

}

TilesModel::Cell::Cell(int r, int c) :
    row(r),
    col(c)
{

}

TilesModel::Cell::Cell(const TilesModel::Cell &cell):
    row(cell.row),
    col(cell.col)
{

}

TilesModel::Cell::Cell(int index)
{
    row = std::ceil(index / width_) + 1;
    col = (index + 1) - ((row - 1) * width_);
}

bool TilesModel::Cell::operator ==(const TilesModel::Cell cell)
{
    if ((row == cell.row) && (col == cell.col))
        return true;

    return false;
}

bool TilesModel::Cell::valid() const
{
    if ((row <= 0) || (row > width_))
        return false;

    if ((col <= 0) || (col > height_))
        return false;

    return true;
}

int TilesModel::Cell::getIndex() const
{
    return ((row - 1) * width_) + col - 1;
}

void TilesModel::Cell::setStaticParams(int width, int height)
{
    width_ = width;
    height_ = height;
}

///////////////////////////////////////////////////////////////////////////////////////////


TilesModel::TilesModel() :
    width_(8),
    height_(12),
    draged_cell_()
{
    Cell::setStaticParams(width_, height_);

    int dim_size = width_ * height_;

    for (int i = 0; i < dim_size; i++) {
        int tile_type = std::rand() % 3;
        switch (tile_type) {
        case 0:
            data_list_.push_back(new Tile(this, "yellow", "rectangle"));
            break;
        case 1:
            data_list_.push_back(new Tile(this, "green", "triangle"));
            break;
        case 2:
            data_list_.push_back(new Tile(this, "blue", "circle"));
            break;
        default:
            break;
        }
    }
}

TilesModel *TilesModel::Instance()
{
    static TilesModel theSingleInstance;
    return &theSingleInstance;
}

int TilesModel::rowCount(const QModelIndex & parent) const {
    return data_list_.size();
}


QVariant TilesModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > data_list_.size())
        return QVariant();

    const Tile *el = data_list_[index.row()];

    switch (role) {
    case ColorRole:
        return el->getColor();
        break;
    case TypeRole:
        return el->getType();
        break;
    default:
        return QVariant();
        break;
    }
}


QHash<int, QByteArray> TilesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ColorRole] = "color";
    roles[TypeRole] = "form";
    return roles;
}

bool TilesModel::able_to_move(Cell target_cell)
{
    // we can move only one cell to right, left, up or down
    std::vector<Cell> cells_to_move = cellsToMove(draged_cell_);
    std::vector<Cell>::iterator it = std::find(cells_to_move.begin(), cells_to_move.end(), target_cell);
    if (it == cells_to_move.end())
        return false;

    // we cen move only in case of matchings after move
//    Tile *target_tile = data_list_[target_cell];
//    Tile *draget_tile = data_list_[draged_cell_];


    return true;

}

std::vector<TilesModel::Cell> TilesModel::cellsToMove(TilesModel::Cell p)
{
    std::vector<Cell> res;

    Cell cell;

    cell = Cell(p.row, p.col - 1);
    if (cell.valid())
        res.push_back(cell);

    cell = Cell(p.row, p.col + 1);
    if (cell.valid())
        res.push_back(cell);

    cell = Cell(p.row - 1, p.col);
    if (cell.valid())
        res.push_back(cell);

    cell = Cell(p.row + 1, p.col);
    if (cell.valid())
        res.push_back(cell);


    return res;
}

std::vector<std::vector<TilesModel::Cell> > TilesModel::findMatches() const
{

    for (int row = 1; row > width_; row++) {
        for (int col = 1; col > height_; col++) {

        }
    }
}

void TilesModel::swapCells(const int from, const int to)
{
    if (from == to)
        return;

    if (std::abs(from - to) > 1) {
        int min = std::min(from, to);
        int max = std::max(from, to);

        beginMoveRows(QModelIndex(), max, max, QModelIndex(), min);
        endMoveRows();

        beginMoveRows(QModelIndex(), min+1, min+1, QModelIndex(), max+1);
        endMoveRows();
    } else {
        int add_num = 0;

        if (from < to)
            add_num = 1;

        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + add_num);
        endMoveRows();

    }

    std::swap(data_list_[from], data_list_[to]);
}

void TilesModel::swapCells(const TilesModel::Cell &from, const TilesModel::Cell &to)
{
    swapCells(from.getIndex(), to.getIndex());
}


void TilesModel::moveTile(int index)
{
    Cell curr_cell(index);
    //qDebug() << index << "  |  " << cell.row << "," << cell.col << "  |  " << cell.getIndex();

    if (!draged_cell_.valid()) {
        draged_cell_ = Cell(index);
    } else {
        if (able_to_move(curr_cell)) {
            swapCells(draged_cell_, curr_cell);
        }

        draged_cell_ = Cell();
    }

}


int TilesModel::getWidth()
{
    return width_;
}

void TilesModel::setWidth(const int val)
{
    width_ = val;
    emit widthChanged();
}

int TilesModel::getHeight()
{
    return height_;
}

void TilesModel::setHeight(const int val)
{
    height_ = val;
    emit heightChanged();
}


