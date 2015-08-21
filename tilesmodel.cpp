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

TilesModel::Cell::Cell(const Cell &cell):
    row(cell.row),
    col(cell.col)
{

}

TilesModel::Cell::Cell(int r, int c) :
    row(r),
    col(c)
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
    if ((row <= 0) || (row > height_))
        return false;

    if ((col <= 0) || (col > width_))
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
    width_(5),
    height_(8),
    draged_cell_()
{
    Cell::setStaticParams(width_, height_);

    int dim_size = width_ * height_;

    for (int i = 0; i < dim_size; i++) {
        int tile_type = std::rand() % 3;
        switch (tile_type) {
        case 0:
            data_list_.push_back(new Tile(this, "rectangle", "yellow", 1));
            break;
        case 1:
            data_list_.push_back(new Tile(this, "triangle", "green", 1));
            break;
        case 2:
            data_list_.push_back(new Tile(this, "circle", "blue", 1));
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
    case TypeRole:
        return el->getType();
        break;
    case ColorRole:
        return el->getColor();
        break;
    case OpacityRole:
        return el->getOpacity();
        break;
    default:
        return QVariant();
        break;
    }
}


QHash<int, QByteArray> TilesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[ColorRole] = "color";
    roles[OpacityRole] = "opacity";

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
    std::vector<std::vector<Cell> > res;
    std::vector<Cell> one_match;

    // vertical matches
    for (int col = 1; col <= width_; col++) {
        one_match.push_back(Cell(1, col));

        for (int row = 2; row <= height_; row++) {
            Cell cell(row, col);
            Cell last_cell = one_match[one_match.size() - 1].getIndex();
            Tile *last_tile = data_list_[last_cell.getIndex()];

            if (last_tile->getType() == data_list_[cell.getIndex()]->getType()) {
                one_match.push_back(cell);
            } else {
                if (one_match.size() >= 3) {
                    res.push_back(one_match);
                }

                one_match.clear();
                one_match.push_back(cell);
            }
        }

        if (one_match.size() >= 3)
            res.push_back(one_match);

        one_match.clear();
    }

    // horisontal matches
    for (int row = 1; row <= height_; row++) {
        one_match.push_back(Cell(row, 1));

        for (int col = 2; col <= width_; col++) {
            Cell cell(row, col);
            Cell last_cell = one_match[one_match.size() - 1].getIndex();
            Tile *last_tile = data_list_[last_cell.getIndex()];

            if (last_tile->getType() == data_list_[cell.getIndex()]->getType()) {
                one_match.push_back(cell);
            } else {
                if (one_match.size() >= 3) {
                    res.push_back(one_match);
                }

                one_match.clear();
                one_match.push_back(cell);
            }
        }

        if (one_match.size() >= 3)
            res.push_back(one_match);

        one_match.clear();
    }

    return res;
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
    std::vector< std::vector<Cell> > tmp = findMatches();
    std::vector< std::vector<Cell> >::iterator it1;

    for (it1 = tmp.begin(); it1 != tmp.end(); it1++) {
        std::vector<Cell>::iterator it2;
        for (it2 = it1->begin(); it2 != it1->end(); it2++) {
            Cell curr_cell((*it2));

            beginResetModel();
            data_list_[curr_cell.getIndex()]->setOpacity(0.5);
            endResetModel();
        }
    }

    for (it1 = tmp.begin(); it1 != tmp.end(); it1++) {
        std::vector<Cell>::iterator it2;
        for (it2 = it1->begin(); it2 != it1->end(); it2++) {
            Cell curr_cell((*it2));
            Cell upper_cell(curr_cell.row - 1, curr_cell.col);

            if (data_list_[curr_cell.getIndex()]->getOpacity() == 1)
                continue;

            while (upper_cell.valid()) {
                if (data_list_[upper_cell.getIndex()]->getOpacity() != 1)
                    break;

                swapCells(curr_cell, upper_cell);

                curr_cell = upper_cell;
                upper_cell.row -= 1;
            }
        }
    }




//    Cell curr_cell(index);

//    if (!draged_cell_.valid()) {
//        draged_cell_ = Cell(index);
//    } else {
//        if (able_to_move(curr_cell)) {
//            swapCells(draged_cell_, curr_cell);
//        }

//        draged_cell_ = Cell();
//    }

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


