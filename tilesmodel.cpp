#include "tilesmodel.h"
#include "command.h"

#include <QDebug>

#include <cmath>


bool TilesModel::m_initialised(false);

TilesModel::TilesModel() :
    m_root(NULL),
    m_width(4),
    m_height(6),
    m_movesCount(0),
    m_score(0),
    m_dragedCell()
{
}

TilesModel *TilesModel::Instance()
{
    static TilesModel theSingleInstance;
    return &theSingleInstance;
}

void TilesModel::generate()
{
    if (!m_initialised)
        return;

    int dim_size = m_width * m_height;

    int i;
    for (i = 0; i < dim_size; i++) {
        m_dataList.push_back(QSharedPointer<Tile>(new Tile(getRandType())));

        // to avoid matches on start of game
        while (leadsToMatch(m_dataList[i])) {
            m_dataList[i]->setDefault(getRandType());
        }
    }

    // one more row for invisible part
    dim_size += m_width;
    for (i; i < dim_size; i++) {

        m_dataList.push_back(QSharedPointer<Tile>(new Tile(getRandType())));
    }
}

bool TilesModel::leadsToMatch(QSharedPointer<Tile> new_tile)
{
    Cell lower_cell(Cell(new_tile->index()).lower());
    Cell left_cell(Cell(new_tile->index()).left());

    for (int i = 0; i < 2; i++) {
        if (!lower_cell.valid())
            break;

        if (m_dataList[lower_cell.index()]->type() != new_tile->type())
            break;

        lower_cell = lower_cell.lower();

        // if we still don't occures break, we have match here
        if (i == 1)
            return true;
    }

    for (int i = 0; i < 2; i++) {
        if (!left_cell.valid())
            break;

        if (m_dataList[left_cell.index()]->type() != new_tile->type())
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
    int tile_type = std::rand() % m_types.size();

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

TilesModel::GameResult TilesModel::gameResult()
{
    if (matchesExisting())
        return NotFinished;

    if (m_movesCount >= m_maxMovesCount) {
        if (m_score >= m_minScore) {
            return Win;
        }
        else {
            return Lose;
        }
    }

    return NotFinished;
}

int TilesModel::indexOfItem(const Tile *item) const
{
    for (int i = 0; i < m_dataList.size(); i++) {
        if (m_dataList[i] == item) {
            return i;
        }
    }

    return -1;
}

int TilesModel::rowCount(const QModelIndex & parent) const {
    return m_dataList.size();
}

QVariant TilesModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_dataList.size())
        return QVariant();

    const QSharedPointer<Tile> tile = m_dataList[index.row()];

    switch (role) {
    case TypeRole:
        return tile->type();
        break;
    case OpacityRole:
        return tile->opacity();
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
    roles[OpacityRole] = "opacity";
    roles[ScaleRole] = "scale";

    return roles;
}

bool TilesModel::able_to_move(Cell target_cell)
{
    // we can move only one cell to right, left, up or down
    QVector<Cell> cells_to_move = cellsToMove(m_dragedCell);
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
    for (int col = 1; col <= m_width; col++) {
        one_match.push_back(m_dataList[Cell(1, col).index()]);

        for (int row = 2; row <= m_height; row++) {
            Cell cell(row, col);
            QSharedPointer<Tile> last_tile = one_match.back();
            QSharedPointer<Tile> curr_tile = m_dataList[cell.index()];

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
    for (int row = 1; row <= m_height; row++) {
        one_match.push_back(m_dataList[Cell(row, 1).index()]);

        for (int col = 2; col <= m_width; col++) {
            Cell cell(row, col);
            QSharedPointer<Tile> last_tile = one_match.back();
            QSharedPointer<Tile> curr_tile = m_dataList[cell.index()];

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

    if (m_packList.empty()) {
        createPackages();
        return;
    }

    Package pack = m_packList.dequeue();
    pack.exec();
    setPackDelay(pack.delay());

    emit statusChanged();

    QObject *pack_timer = m_root->findChild<QObject *>("pack_timer");
    QMetaObject::invokeMethod(pack_timer, "start");

    if (m_packList.empty()) {
        QObject *messageDialog = m_root->findChild<QObject *>("messageDialog");
        switch (gameResult()) {
        case Win:
            QMetaObject::invokeMethod(messageDialog, "show", Q_ARG(QVariant, QVariant("Congratulation, you win!!")));
            break;
        case Lose:
            QMetaObject::invokeMethod(messageDialog, "show", Q_ARG(QVariant, QVariant("Sorry, but you lose, try again!!")));
            break;
        default:
            break;
        }
    }
}

void TilesModel::createPackages()
{
    QVector< QVector<QSharedPointer<Tile> > > matches_list = findMatches();

    Package pack;
    for (int i = 0; i < matches_list.size(); i++) {
        QVector<QSharedPointer<Tile> > one_match = matches_list[i];
        for (int j = 0; j < one_match.size(); j++) {
            one_match[j]->setValid(false);

            pack.push(QSharedPointer<Command>(new ScaleCommand(one_match[j], 0.8)));
            pack.push(QSharedPointer<Command>(new OpacityCommand(one_match[j], 0.0)));
            m_score += m_elementScore;
        }
    }

    if (pack.size())
        m_packList.enqueue(pack);


    for (int iteration = 0; iteration <= m_height; iteration++) {
        Package movePack;
        Package createPack;

        for (int col = 1; col <= m_width; col++) {
            int num_in_match = 0;

            for (int row = 1; row <= m_height; row++) {
                Cell cell(row, col);
                QSharedPointer<Tile> tile = m_dataList[cell.index()];

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
            m_packList.enqueue(movePack);

        if (createPack.size())
            m_packList.enqueue(createPack);

    }


    if (m_packList.size())
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

    qSwap(m_dataList[from], m_dataList[to]);
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
    m_dataList.erase(m_dataList.begin() + index);
    endRemoveRows();

    beginInsertRows(QModelIndex(), index, index);
    m_dataList.insert(m_dataList.begin() + index, QSharedPointer<Tile>(new Tile(getRandType())));
    endInsertRows();
}

void TilesModel::refreshItem(int index)
{
    beginResetModel();
    m_dataList[index]->setDefault(getRandType());
    endResetModel();
}

QSharedPointer<Tile> TilesModel::item(int index)
{
    return m_dataList[index];
}

void TilesModel::moveTile(int index)
{
    Cell curr_cell(index);

    // for providing animated scale effect for draged tile
    QObject *scale_timer = m_root->findChild<QObject *>("scale_timer");

    if (!m_dragedCell.valid()) {
        m_dragedCell = Cell(index);

        QMetaObject::invokeMethod(scale_timer, "start");

    } else {
        if (able_to_move(curr_cell)) {
            changeScale(m_dataList[m_dragedCell.index()], 1);
            QMetaObject::invokeMethod(scale_timer, "stop");

            qSwap(m_dataList[curr_cell.index()], m_dataList[m_dragedCell.index()]);
            bool matches = matchesExisting();
            qSwap(m_dataList[curr_cell.index()], m_dataList[m_dragedCell.index()]);

            if (matches) {
                Package pack;
                pack.push(QSharedPointer<Command>(new SwapCommand(curr_cell, m_dragedCell)));
                m_packList.enqueue(pack);

                m_movesCount++;
                emit statusChanged();

                createPackages();
            } else {
                Package pack;

                pack.push(QSharedPointer<Command>(new SwapCommand(curr_cell, m_dragedCell)));
                m_packList.enqueue(pack);
                pack.clear();

                pack.push(QSharedPointer<Command>(new SwapCommand(curr_cell, m_dragedCell)));
                m_packList.enqueue(pack);

                execNextPackage();

            }

            m_dragedCell = Cell();

        } else {
            changeScale(m_dataList[m_dragedCell.index()], 1);
            m_dragedCell = Cell(index);
        }

    }
}

void TilesModel::provideScaleAnimation()
{
    if (!m_dragedCell.valid())
        return;

    QSharedPointer<Tile> tile = m_dataList[m_dragedCell.index()];
    float scale = tile->scale();
    if (scale > 1) {
        scale = scale / 1.2;
    } else {
        scale = scale * 1.2;
    }

    changeScale(tile, scale);
}


int TilesModel::width()
{
    return m_width;
}

void TilesModel::setWidth(const int width)
{
    m_width = width;
    emit widthChanged();
}

int TilesModel::height()
{
    return m_height;
}

void TilesModel::setHeight(const int height)
{
    m_height = height;
    emit heightChanged();
}

QString TilesModel::status()
{
    return QString(" score: %1; moves: %2").arg(m_score).arg(m_movesCount);
}

bool TilesModel::getInitialised()
{
    return m_initialised;
}

void TilesModel::setInitialised(bool initialised)
{
    m_initialised = initialised;
}

QVector<int> TilesModel::getTypes() const
{
    return m_types;
}

void TilesModel::setTypes(const QVector<int> &types)
{
    m_types.clear();
    for (int i = 0; i < types.size(); i++)
        m_types.push_back(types[i]);
}

int TilesModel::maxMoves() const
{
    return m_maxMovesCount;
}

void TilesModel::setMaxMoves(int maxMoves)
{
    m_maxMovesCount = maxMoves;
}

int TilesModel::minScore() const
{
    return m_minScore;
}

void TilesModel::setMinScore(int minScore)
{
    m_minScore = minScore;
}

int TilesModel::elementScore() const
{
    return m_elementScore;
}

void TilesModel::setElementScore(int element_score)
{
    m_elementScore = element_score;
}

int TilesModel::packDelay() const
{
    return m_packDelay;
}

void TilesModel::setPackDelay(int packDelay)
{
    m_packDelay = packDelay;
    emit packDelayChanged();
}

QObject *TilesModel::getRoot() const
{
    return m_root;
}

void TilesModel::setRoot(QObject *root)
{
    m_root = root;
}
