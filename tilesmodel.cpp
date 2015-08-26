#include "tilesmodel.h"
#include "command.h"

#include <QDebug>
#include <QTimer>
#include <QThread>

#include <cmath>


bool TilesModel::initialised_(false);

TilesModel::TilesModel() :
    width_(4),
    height_(6),
    draged_cell_()
{
}

QString TilesModel::getRandType()
{
    int tile_type = std::rand() % types_.size();
    switch (tile_type) {
    case 0:
        return "yellow";
    case 1:
        return "red";
    case 2:
        return "blue";
    case 3:
        return "green";
    case 4:
        return "black";
    case 5:
        return "pink";
    default:
        return "";
    }
}
bool TilesModel::getInitialised()
{
    return initialised_;
}

void TilesModel::setInitialised(bool initialised)
{
    initialised_ = initialised;
}

std::vector<int> TilesModel::getTypes() const
{
    return types_;
}

void TilesModel::setTypes(const std::vector<int> &types)
{
    types_.clear();
    for (int i = 0; i < types.size(); i++)
        types_.push_back(types[i]);
}

int TilesModel::getMax_moves() const
{
    return max_moves_;
}

void TilesModel::setMax_moves(int max_moves)
{
    max_moves_ = max_moves;
}

int TilesModel::getMin_score() const
{
    return min_score_;
}

void TilesModel::setMin_score(int min_score)
{
    min_score_ = min_score;
}

int TilesModel::getElement_score() const
{
    return element_score_;
}

void TilesModel::setElement_score(int element_score)
{
    element_score_ = element_score;
}



TilesModel *TilesModel::Instance()
{
    static TilesModel theSingleInstance;
    return &theSingleInstance;
}

void TilesModel::generate()
{
    if (!initialised_)
        return;

    int dim_size = width_ * height_;

    for (int i = 0; i < dim_size; i++) {
        QString rand_type = getRandType();
        data_list_.push_back(new Tile(this, rand_type, rand_type, i, 1));
    }
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
        return el->type();
        break;
    case ColorRole:
        return el->color();
        break;
    case OpacityRole:
        return el->opacity();
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

}

bool TilesModel::able_to_move(Cell target_cell)
{
    // we can move only one cell to right, left, up or down
    std::vector<Cell> cells_to_move = cellsToMove(draged_cell_);
    std::vector<Cell>::iterator it = std::find(cells_to_move.begin(), cells_to_move.end(), target_cell);
    if (it == cells_to_move.end())
        return false;


    return true;
}

std::vector<Cell> TilesModel::cellsToMove(Cell cell)
{
    std::vector<Cell> res;

    if (cell.right().valid())
        res.push_back(cell.right());

    if (cell.left().valid())
        res.push_back(cell.left());

    if (cell.upper().valid())
        res.push_back(cell.upper());

    if (cell.lower().valid())
        res.push_back(cell.lower());


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

            if (last_tile->type() == data_list_[cell.index()]->type()) {
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

//            // avoiding processing the same tile twice
//            bool repit = false;
//            for (std::vector<std::vector<Tile *> >::iterator it1 = res.begin(); it1 < res.end(); it1++) {
//                std::vector<Tile *>::iterator it2 = std::find(it1->begin(), it1->end(), data_list_[cell.index()]);
//                if (it2 == it1->end())
//                    repit = true;
//            }
//            if (repit)
//                continue;


            if (last_tile->type() == data_list_[cell.index()]->type()) {
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

void TilesModel::createPackages()
{
    std::vector< std::vector<Tile *> > matches_list = findMatches();
    std::vector< std::vector<Tile *> >::iterator it1;


    for (it1 = matches_list.begin(); it1 != matches_list.end(); it1++) {
        std::vector<Tile *>::iterator it2;

        std::vector<Package> tmp_createPack;

        Package opacityPack;
        Package movePack;
        Package createPack;

        for (it2 = it1->begin(); it2 != it1->end(); it2++) {
            opacityPack.clear();
            opacityPack.push(new OpacityCommand(*it2, 0.0));
            pack_list_.push(opacityPack);

            movePack.push(new MoveUpCommand(*it2));

            createPack.push(new CreateCommand(*it2));
            tmp_createPack.push_back(createPack);
            createPack.clear();
        }

//        pack_list_.push(opacityPack);
        pack_list_.push(movePack);
//        pack_list_.push(createPack);
        for (int i = tmp_createPack.size() - 1; i >= 0 ; i--) {
            pack_list_.push(tmp_createPack[i]);
        }


    }

    if (pack_list_.size())
        execNextPackage();

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
    int tmp_index = data_list_[from]->index();
    data_list_[from]->setIndex(data_list_[to]->index());
    data_list_[to]->setIndex(tmp_index);

}

void TilesModel::swapCells(const Cell &from, const Cell &to)
{
    swapCells(from.index(), to.index());
}

void TilesModel::changeOpacity(Tile *target, const float opacity)
{
    target->setOpacity(opacity);

    QVector<int> vec;
    vec.push_back(OpacityRole);
    emit dataChanged(createIndex(target->index(), 0), createIndex(target->index(), 0), vec);

}

void TilesModel::createNewItem(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    data_list_.erase(data_list_.begin() + index);
    endRemoveRows();

    beginInsertRows(QModelIndex(), index, index);
    QString type = getRandType();
    data_list_.insert(data_list_.begin() + index, new Tile(this, type, type, index, 1));
    endInsertRows();
}

Tile *TilesModel::item(int index)
{
    return data_list_[index];
}

void TilesModel::moveTile(int index)
{
    Cell curr_cell(index);

    if (!draged_cell_.valid()) {
        draged_cell_ = Cell(index);
    } else {
        if (able_to_move(curr_cell)) {
            swapCells(draged_cell_, curr_cell);
            createPackages();
        }

        draged_cell_ = Cell();
    }

}

void TilesModel::execNextPackage()
{
    if (pack_list_.empty()) {
//        createPackages();
        return;
    }

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


