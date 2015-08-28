#include "tilesmodel.h"
#include "command.h"

#include <QDebug>

#include <cmath>


bool TilesModel::initialised_(false);

TilesModel::TilesModel() :
    exec_pack_cnt_(0),
    width_(4),
    height_(6),
    draged_cell_()
{
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
        data_list_.push_back(new Tile(getRandType()));
    }

//    dim_size += width_ * height_;

    for (int i = width_ * height_; i < dim_size + width_; i++) {
//        data_list_.push_back(new Tile());
        data_list_.push_back(new Tile(getRandType()));

    }
}

QString TilesModel::getRandType()
{
    int tile_type = std::rand() % types_.size();

    switch (tile_type) {
    case 0:
        return "qrc:/icon/icon/img1.png";
    case 1:
        return "qrc:/icon/icon/img2.png";
    case 2:
        return "qrc:/icon/icon/img3.png";
    case 3:
        return "qrc:/icon/icon/img4.png";
    case 4:
        return "qrc:/icon/icon/img5.png";
    case 5:
        return "qrc:/icon/icon/img6.png";
    case 6:
        return "qrc:/icon/icon/img7.png";
    case 7:
        return "qrc:/icon/icon/img8.png";
    case 8:
        return "qrc:/icon/icon/img9.png";
    case 9:
        return "qrc:/icon/icon/img10.png";
    default:
        return "";
    }
}

bool TilesModel::checkForRepeating(Tile *tile, std::vector<std::vector<Tile *> > conteiner) const
{
    for (std::vector<std::vector<Tile *> >::iterator it1 = conteiner.begin(); it1 < conteiner.end(); it1++) {
        for (std::vector<Tile *>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
            if (*it2 == tile) {
                return true;
            }
        }
    }
}

bool TilesModel::matchesExisting()
{
    std::vector< std::vector<Tile *> > matches_list = findMatches();
    if (matches_list.empty())
        return false;

    return true;
}

int TilesModel::indexOfItem(const Tile *item) const
{
    for (int i = 0; i < data_list_.size(); i++) {
        if (data_list_[i] == item) {
            return i;
        }
    }

    return -1;
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
    case TextRole:
        return el->text();
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
    roles[TextRole] = "text";

    return roles;
}

void TilesModel::someSlot()
{
    std::vector< std::vector<Tile *> > matches_list = findMatches();
    qDebug() << matches_list.size();
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
            Tile *last_tile = one_match.back();
            Tile *curr_tile = data_list_[cell.index()];

            if (last_tile->type() == curr_tile->type()) {
                one_match.push_back(curr_tile);
            } else {
                if (one_match.size() >= 3) {
                    res.push_back(one_match);
                }

                one_match.clear();
                one_match.push_back(curr_tile);
            }
        }

        if (one_match.size() >= 3)
            res.push_back(one_match);

        one_match.clear();
    }

    // horisontal matches
    std::vector<std::vector<Tile *> > hor_res;
    for (int row = 1; row <= height_; row++) {
        one_match.push_back(data_list_[Cell(row, 1).index()]);

        for (int col = 2; col <= width_; col++) {
            Cell cell(row, col);
            Tile *last_tile = one_match.back();
            Tile *curr_tile = data_list_[cell.index()];

            if (last_tile->type() == curr_tile->type()) {
                one_match.push_back(curr_tile);
            } else {
                if (one_match.size() >= 3) {
                    hor_res.push_back(one_match);
                }

                one_match.clear();
                one_match.push_back(curr_tile);
            }
        }

        if (one_match.size() >= 3)
            hor_res.push_back(one_match);

        one_match.clear();
    }

    // removing doubles to avoid processing the same tile twice
    for (int i = 0; i < hor_res.size(); i++) {
        std::vector<Tile *> one_hor_match = hor_res[i];
        for (int j = 0; j < one_hor_match.size(); j++) {
            if (checkForRepeating(one_hor_match[j], res)) {
                one_hor_match.erase(one_hor_match.begin() + j);
            }
        }

        res.push_back(one_hor_match);
    }



    return res;
}


void TilesModel::execNextPackage()
{
    if (exec_pack_cnt_ != 0)
        return;

    if (pack_list_.empty()) {
        createPackages();
        return;
    }

    Package pack = pack_list_.front();
    pack_list_.pop();

    pack.exec();
}

void TilesModel::createPackages()
{
    std::vector< std::vector<Tile *> > matches_list = findMatches();

    for (int i = 0; i < matches_list.size(); i++) {
        std::vector<Tile *> one_match = matches_list[i];
        for (int j = 0; j < one_match.size(); j++) {
            beginResetModel();
            one_match[j]->setText("X");
            endResetModel();
        }
    }


    std::vector< std::vector<Tile *> >::iterator it1;
    for (it1 = matches_list.begin(); it1 != matches_list.end(); it1++) {
        std::vector<Tile *>::iterator it2;

        std::vector<Package> tmp_movePack;
        std::vector<Package> tmp_createPack;

        Package opacityPack;
        Package movePack;
        Package createPack;

        bool horizontal_match = false;
        if ((*it1).size() > 1) {
            Cell first_cell((*it1)[0]->index());
            Cell second_cell((*it1)[1]->index());
            if (first_cell.col() != second_cell.col()) {
                horizontal_match = true;
            }
        }

        for (it2 = it1->begin(); it2 != it1->end(); it2++) {
            opacityPack.push(new OpacityCommand(*it2, 0.0));


            if (horizontal_match) {
                movePack.push(new MoveUpCommand(*it2));
                createPack.push(new CreateCommand(*it2));
            } else {
                movePack.push(new MoveUpCommand(*it2));
                tmp_movePack.push_back(movePack);
                movePack.clear();

                createPack.push(new CreateCommand(*it2));
                tmp_createPack.push_back(createPack);
                createPack.clear();
            }

        }

        pack_list_.push(opacityPack);

        if (horizontal_match) {
            pack_list_.push(movePack);
            pack_list_.push(createPack);
        } else {
            for (int i = 0; i < tmp_movePack.size(); i++) {
                pack_list_.push(tmp_movePack[i]);
                pack_list_.push(tmp_createPack[i]);
            }
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
    beginResetModel();
    data_list_[index]->setDefault(getRandType());
    endResetModel();
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
            std::swap(data_list_[curr_cell.index()], data_list_[draged_cell_.index()]);
            bool matches = matchesExisting();
            std::swap(data_list_[curr_cell.index()], data_list_[draged_cell_.index()]);

            if (matches) {
                Package pack;
                pack.push(new SwapCommand(curr_cell, draged_cell_));
                pack_list_.push(pack);

                createPackages();
            } else {
                Package pack;

                pack.push(new SwapCommand(curr_cell, draged_cell_));
                pack_list_.push(pack);
                pack.clear();

                pack.push(new SwapCommand(curr_cell, draged_cell_));
                pack_list_.push(pack);

                execNextPackage();

            }

//            swapCells(draged_cell_, curr_cell);

//            if (matchesExisting()) {
//                createPackages();
//            } else {
//                swapCells(draged_cell_, curr_cell);
//            }
        }

        draged_cell_ = Cell();
    }


}


int TilesModel::width()
{
    return width_;
}

void TilesModel::setWidth(const int width)
{
    width_ = width;
    emit widthChanged();
}

int TilesModel::height()
{
    return height_;
}

void TilesModel::setHeight(const int height)
{
    height_ = height;
    emit heightChanged();
}

int TilesModel::execPackCnt() const
{
    return exec_pack_cnt_;
}

void TilesModel::setExecPackCnt(int exec_pack_cnt)
{
    exec_pack_cnt_ = exec_pack_cnt;
    emit execPackCntChanged();
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


