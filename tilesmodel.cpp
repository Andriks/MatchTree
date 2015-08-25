#include "tilesmodel.h"
#include "command.h"

#include <QDebug>
#include <QTimer>
#include <QThread>

#include <cmath>


TilesModel::TilesModel() :
    width_(6),
    height_(8),
    draged_cell_()
{
    int dim_size = width_ * height_;

    for (int i = 0; i < dim_size; i++) {
        QString rand_type = getRandType();
        data_list_.push_back(new Tile(this, rand_type, rand_type, i, 1));
    }
}

QString TilesModel::getRandType()
{
    int tile_type = std::rand() % 3;
    switch (tile_type) {
    case 0:
        return "yellow";
    case 1:
        return "green";
    case 2:
        return "blue";
    default:
        return "";
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

void TilesModel::someSlot()
{
    Package pack;

    for (int i = 0; i < 4; i++) {
        pack.push(new MoveCommand(Cell(i), Cell(i+1)));
    }

    pack_list_.push(pack);
    pack.clear();

    for (int i = 30; i < 35; i++) {
        pack.push(new MoveCommand(Cell(i), Cell(i+1)));
    }

    pack_list_.push(pack);

    for (int i = 20; i < 23; i++) {
        pack.clear();
        pack.push(new OpacityCommand(Cell(i), 0));
        pack_list_.push(pack);
    }



//    pack_list_.push(pack);
    execNextPackage();
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

std::vector<Cell> TilesModel::cellsToMove(Cell p)
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

std::vector<std::vector<Tile *> > TilesModel::findMatches() const
{
    std::vector<std::vector<Tile *> > res;
    std::vector<Tile *> one_match;

    // vertical matches
    for (int col = 1; col <= width_; col++) {
        one_match.push_back(data_list_[Cell(1, col).index()]);

        for (int row = 2; row <= height_; row++) {
            Cell cell(row, col);
            Tile *last_tile = one_match[one_match.size() - 1];

            if (last_tile->getType() == data_list_[cell.index()]->getType()) {
                one_match.push_back(data_list_[cell.index()]);
            } else {
                if (one_match.size() >= 3) {
                    res.push_back(one_match);
                }

                one_match.clear();
                one_match.push_back(data_list_[cell.index()]);
            }
        }

        if (one_match.size() >= 3)
            res.push_back(one_match);

        one_match.clear();
    }

    // horisontal matches
    for (int row = 1; row <= height_; row++) {
        one_match.push_back(data_list_[Cell(row, 1).index()]);

        for (int col = 2; col <= width_; col++) {
            Cell cell(row, col);
            Tile *last_tile = one_match[one_match.size() - 1];

            if (last_tile->getType() == data_list_[cell.index()]->getType()) {
                one_match.push_back(data_list_[cell.index()]);
            } else {
                if (one_match.size() >= 3) {
                    res.push_back(one_match);
                }

                one_match.clear();
                one_match.push_back(data_list_[cell.index()]);
            }
        }

        if (one_match.size() >= 3)
            res.push_back(one_match);

        one_match.clear();
    }

    return res;
}

void TilesModel::removeMatches()
{
    std::vector< std::vector<Tile *> > matches_list = findMatches();
    std::vector< std::vector<Tile *> >::iterator it1;

    while (matches_list.size()) {

        for (it1 = matches_list.begin(); it1 != matches_list.end(); it1++) {
            std::vector<Tile *>::iterator it2;
            for (it2 = it1->begin(); it2 != it1->end(); it2++) {
                Cell curr_cell = Cell((*it2)->getIndex());
                Cell upper_cell(curr_cell.row - 1, curr_cell.col);

                beginResetModel();
                data_list_[curr_cell.index()]->setOpacity(0.5);
                endResetModel();

                while (upper_cell.valid()) {
                    if (data_list_[upper_cell.index()]->getOpacity() != 1)
                        break;

                    swapCells(curr_cell, upper_cell);

                    curr_cell = upper_cell;
                    upper_cell.row -= 1;
                }

                delete data_list_[curr_cell.index()];
                QString rand_type = getRandType();

                beginResetModel();
                data_list_[curr_cell.index()] = new Tile(this, rand_type, rand_type, curr_cell.index(), 1);
                endResetModel();
            }
        }

        matches_list.clear();
        matches_list = findMatches();
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

//    qDebug() << data_list_[from]->getIndex() << "  " << data_list_[from]->getColor() << " | " << data_list_[to]->getIndex()<< "  " << data_list_[to]->getColor();

    std::swap(data_list_[from], data_list_[to]);
    int tmp_index = data_list_[from]->getIndex();
    data_list_[from]->setIndex(data_list_[to]->getIndex());
    data_list_[to]->setIndex(tmp_index);

    //QTimer::singleShot(1000);
//    QThread::sleep(1);

//    qDebug() << data_list_[from]->getIndex() << "  " << data_list_[from]->getColor() << " | " << data_list_[to]->getIndex()<< "  " << data_list_[to]->getColor();
}

void TilesModel::swapCells(const Cell &from, const Cell &to)
{
    swapCells(from.index(), to.index());
}

void TilesModel::changeOpacity(const Cell &target, const float opacity)
{
    beginResetModel();
    data_list_[target.index()]->setOpacity(opacity);
    endResetModel();
}


void TilesModel::moveTile(int index)
{
    Cell curr_cell(index);

    if (!draged_cell_.valid()) {
        draged_cell_ = Cell(index);
    } else {
        if (able_to_move(curr_cell)) {
            swapCells(draged_cell_, curr_cell);
            removeMatches();
        }

        draged_cell_ = Cell();
    }

}

void TilesModel::execNextPackage()
{
    if (pack_list_.empty())
        return;

    Package pack = pack_list_.front();
    pack_list_.pop();

    pack.exec();
}


int TilesModel::width()
{
    return width_;
}

void TilesModel::setWidth(const int val)
{
    width_ = val;
    emit widthChanged();
}

int TilesModel::height()
{
    return height_;
}

void TilesModel::setHeight(const int val)
{
    height_ = val;
    emit heightChanged();
}


