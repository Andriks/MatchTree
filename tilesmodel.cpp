#include "tilesmodel.h"
#include "command.h"

#include <QDebug>

#include <cmath>


bool TilesModel::initialised_(false);

TilesModel::TilesModel() :
    root_(NULL),
    exec_pack_cnt_(0),
    moves_cnt_(0),
    score_(0),
    width_(4),
    height_(6),
    draged_cell_()
{
}

QObject *TilesModel::getRoot() const
{
    return root_;
}

void TilesModel::setRoot(QObject *root)
{
    root_ = root;
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

    int i;
    for (i = 0; i < dim_size; i++) {
        data_list_.push_back(QSharedPointer<Tile>(new Tile(getRandType())));

        // to avoid matches on start of game
        while (leadsToMatch(data_list_[i])) {
            data_list_[i]->setDefault(getRandType());
        }
    }

    // one more row for invisible part
    dim_size += width_;
    for (i; i < dim_size; i++) {

        data_list_.push_back(QSharedPointer<Tile>(new Tile(getRandType())));
    }
}

bool TilesModel::leadsToMatch(QSharedPointer<Tile> new_tile)
{
    Cell lower_cell(Cell(new_tile->index()).lower());
    Cell left_cell(Cell(new_tile->index()).left());

    for (int i = 0; i < 2; i++) {
        if (!lower_cell.valid())
            break;

        if (data_list_[lower_cell.index()]->type() != new_tile->type())
            break;

        lower_cell = lower_cell.lower();

        // if we still don't occures break, we have match here
        if (i == 1)
            return true;
    }

    for (int i = 0; i < 2; i++) {
        if (!left_cell.valid())
            break;

        if (data_list_[left_cell.index()]->type() != new_tile->type())
            break;

        left_cell = left_cell.left();

        // if we still don't occures break, we have match here
        if (i == 1)
            return true;
    }

    return false;
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

bool TilesModel::checkForRepeating(QSharedPointer<Tile> tile, QVector<QVector<QSharedPointer<Tile> > > conteiner) const
{
    for (QVector<QVector<QSharedPointer<Tile> > >::iterator it1 = conteiner.begin(); it1 < conteiner.end(); it1++) {
        for (QVector<QSharedPointer<Tile> >::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
            if (*it2 == tile) {
                return true;
            }
        }
    }
}

bool TilesModel::matchesExisting()
{
    QVector< QVector<QSharedPointer<Tile> > > matches_list = findMatches();
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

    const QSharedPointer<Tile> tile = data_list_[index.row()];

    switch (role) {
    case TypeRole:
        return tile->type();
        break;
    case ColorRole:
        return tile->color();
        break;
    case OpacityRole:
        return tile->opacity();
        break;
    case TextRole:
        return tile->text();
        break;
    case ScaleRole:
        return tile->scale();
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
    roles[ScaleRole] = "scale";

    return roles;
}

void TilesModel::someSlot(int index)
{
    QSharedPointer<Tile> tile = data_list_[index];
    float scale = tile->scale();
    if (scale > 1) {
        scale = scale / 1.5;
    } else {
        scale = scale * 1.5;
    }

    changeScale(tile, scale);
}

bool TilesModel::able_to_move(Cell target_cell)
{
    // we can move only one cell to right, left, up or down
    QVector<Cell> cells_to_move = cellsToMove(draged_cell_);
    QVector<Cell>::iterator it = std::find(cells_to_move.begin(), cells_to_move.end(), target_cell);
    if (it == cells_to_move.end())
        return false;


    return true;
}

QVector<Cell> TilesModel::cellsToMove(Cell cell)
{
    QVector<Cell> res;

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

QVector<QVector<QSharedPointer<Tile> > > TilesModel::findMatches() const
{
    QVector<QVector<QSharedPointer<Tile> > > res;
    QVector<QSharedPointer<Tile> > one_match;

    // vertical matches
    for (int col = 1; col <= width_; col++) {
        one_match.push_back(data_list_[Cell(1, col).index()]);

        for (int row = 2; row <= height_; row++) {
            Cell cell(row, col);
            QSharedPointer<Tile> last_tile = one_match.back();
            QSharedPointer<Tile> curr_tile = data_list_[cell.index()];

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
    QVector<QVector<QSharedPointer<Tile> > > hor_res;
    for (int row = 1; row <= height_; row++) {
        one_match.push_back(data_list_[Cell(row, 1).index()]);

        for (int col = 2; col <= width_; col++) {
            Cell cell(row, col);
            QSharedPointer<Tile> last_tile = one_match.back();
            QSharedPointer<Tile> curr_tile = data_list_[cell.index()];

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
        QVector<QSharedPointer<Tile> > one_hor_match = hor_res[i];
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
    pack_list_.dequeue();

    emit statusChanged();

    pack.exec();
}

void TilesModel::createPackages()
{
    QVector< QVector<QSharedPointer<Tile> > > matches_list = findMatches();

    Package opacityPack;
    for (int i = 0; i < matches_list.size(); i++) {
        QVector<QSharedPointer<Tile> > one_match = matches_list[i];
        for (int j = 0; j < one_match.size(); j++) {
            beginResetModel();
            one_match[j]->setText("X");
            one_match[j]->setValid(false);
            endResetModel();

            opacityPack.push(QSharedPointer<Command>(new OpacityCommand(one_match[j], 0.0)));
            score_ += element_score_;
        }
    }

    if (opacityPack.size())
        pack_list_.enqueue(opacityPack);


    for (int iteration = 0; iteration <= height_; iteration++) {
        Package movePack;
        Package createPack;

        for (int col = 1; col <= width_; col++) {
            int num_in_match = 0;

            for (int row = 1; row <= height_; row++) {
                Cell cell(row, col);
                QSharedPointer<Tile> tile = data_list_[cell.index()];

                if (!tile->valid()) {
                    if (num_in_match ==  iteration) {
                        movePack.push(QSharedPointer<Command>(new MoveUpCommand(tile)));
                        createPack.push(QSharedPointer<Command>(new RefreshCommand(tile)));

                        break;
                    }

                    num_in_match++;
                }
            }
        }

        if (movePack.size())
            pack_list_.enqueue(movePack);

        if (createPack.size())
            pack_list_.enqueue(createPack);

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

void TilesModel::changeOpacity(QSharedPointer<Tile> target, const float opacity)
{
    target->setOpacity(opacity);

    QVector<int> vec;
    vec.push_back(OpacityRole);
    emit dataChanged(createIndex(target->index(), 0), createIndex(target->index(), 0), vec);

}

void TilesModel::changeScale(QSharedPointer<Tile> target, const float scale)
{
    target->setScale(scale);

    QVector<int> vec;
    vec.push_back(ScaleRole);
    emit dataChanged(createIndex(target->index(), 0), createIndex(target->index(), 0), vec);
}

void TilesModel::createItem(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    data_list_.erase(data_list_.begin() + index);
    endRemoveRows();

    beginInsertRows(QModelIndex(), index, index);
    data_list_.insert(data_list_.begin() + index, QSharedPointer<Tile>(new Tile(getRandType())));
    endInsertRows();
}

void TilesModel::refreshItem(int index)
{
    beginResetModel();
    data_list_[index]->setDefault(getRandType());
    endResetModel();
}

QSharedPointer<Tile> TilesModel::item(int index)
{
    return data_list_[index];
}

void TilesModel::moveTile(int index)
{
    if (exec_pack_cnt_ != 0)
        return;

    Cell curr_cell(index);

    // for providing animated scale effect for draged tile
    QObject *scale_timer = root_->findChild<QObject *>("scale_timer");

    if (!draged_cell_.valid()) {
        draged_cell_ = Cell(index);

        QMetaObject::invokeMethod(scale_timer, "start");

    } else {
        if (able_to_move(curr_cell)) {
            changeScale(data_list_[draged_cell_.index()], 1);
            QMetaObject::invokeMethod(scale_timer, "stop");

            std::swap(data_list_[curr_cell.index()], data_list_[draged_cell_.index()]);
            bool matches = matchesExisting();
            std::swap(data_list_[curr_cell.index()], data_list_[draged_cell_.index()]);

            if (matches) {
                Package pack;
                pack.push(QSharedPointer<Command>(new SwapCommand(curr_cell, draged_cell_)));
                pack_list_.enqueue(pack);

                moves_cnt_++;
                emit statusChanged();

                createPackages();
            } else {
                Package pack;

                pack.push(QSharedPointer<Command>(new SwapCommand(curr_cell, draged_cell_)));
                pack_list_.enqueue(pack);
                pack.clear();

                pack.push(QSharedPointer<Command>(new SwapCommand(curr_cell, draged_cell_)));
                pack_list_.enqueue(pack);

                execNextPackage();

            }

            draged_cell_ = Cell();

        } else {
            changeScale(data_list_[draged_cell_.index()], 1);
            draged_cell_ = Cell(index);
        }

    }
}

void TilesModel::provideScaleAnimation()
{
    if (!draged_cell_.valid())
        return;

    QSharedPointer<Tile> tile = data_list_[draged_cell_.index()];
    float scale = tile->scale();
    if (scale > 1) {
        scale = scale / 1.5;
    } else {
        scale = scale * 1.5;
    }

    changeScale(tile, scale);
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

QString TilesModel::status()
{
    return QString(" score: %1; moves: %2").arg(score_).arg(moves_cnt_);
}

bool TilesModel::getInitialised()
{
    return initialised_;
}

void TilesModel::setInitialised(bool initialised)
{
    initialised_ = initialised;
}

QVector<int> TilesModel::getTypes() const
{
    return types_;
}

void TilesModel::setTypes(const QVector<int> &types)
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


