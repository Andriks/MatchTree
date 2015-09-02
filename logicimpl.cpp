#include "logicimpl.h"
#include "tilesmodel.h"
#include "jsonparser.h"

#include <cmath>

#include <QString>


LogicImpl::LogicImpl(QObject *parent) :
    QObject(parent),
    m_width(6),
    m_height(6),
    m_elementScore(100),
    m_minScore(1000),
    m_maxMovesCount(5),
    m_movesCount(0),
    m_score(0),
    m_dragedCell(),
    m_packDelay(0)
{    JsonParser parser;
    parser.parse_config("../match3/config.json", this);
}

void LogicImpl::newGame()
{
    if (!m_packList.empty())
        return;

    m_dataList.clear();

    m_movesCount = 0;
    m_score = 0;
    m_dragedCell = Cell();
    m_packDelay = 0;

    emit statusChanged();

    int dim_size = m_width * (m_height + 1); // 1 additional row for invisible part

    int i;
    for (i = 0; i < dim_size; i++) {
        // we are doing insert for suplying animation
        m_dataList.insert(m_dataList.begin() + i, QSharedPointer<Tile>(new Tile(getRandType())));

        // to avoid matches on start of game
        while (leadsToMatch(m_dataList[i])) {
            m_dataList[i]->setDefault(getRandType());
        }
    }
}



bool LogicImpl::leadsToMatch(QSharedPointer<Tile> new_tile) {
    Cell lower_cell(Cell(new_tile->index()).lower());
    Cell left_cell(Cell(new_tile->index()).left());

    for (int i = 0; i < 2; i++) {
        if (!lower_cell.valid())
            break;

        if (m_dataList[lower_cell.index()]->type() != new_tile->type())
            break;

        lower_cell = lower_cell.lower();

        // if we still don't occuresR break, we have match here
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

void LogicImpl::createPackages() {
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

QString LogicImpl::getRandType() {
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

bool LogicImpl::able_to_move(Cell target_cell) {
    // we can move only one cell to right, left, up or down
    QVector<Cell> cells_to_move = cellsToMove(m_dragedCell);
    QVector<Cell>::iterator it = std::find(cells_to_move.begin(), cells_to_move.end(), target_cell);
    if (it == cells_to_move.end())
        return false;


    return true;
}

QVector<Cell> LogicImpl::cellsToMove(Cell cell) {
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

QVector<QVector<QSharedPointer<Tile> > > LogicImpl::findMatches() const {
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
            }
            else {
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
            }
            else {
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

bool LogicImpl::checkForRepeating(QSharedPointer<Tile> tile, QVector<QVector<QSharedPointer<Tile> > > conteiner) const {
    for (QVector<QVector<QSharedPointer<Tile> > >::iterator it1 = conteiner.begin(); it1 < conteiner.end(); it1++) {
        for (QVector<QSharedPointer<Tile> >::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
            if (*it2 == tile) {
                return true;
            }
        }
    }

    return false;
}

bool LogicImpl::matchesExisting() {
    QVector< QVector<QSharedPointer<Tile> > > matches_list = findMatches();
    if (matches_list.empty())
        return false;

    return true;
}

LogicImpl::GameResult LogicImpl::gameResult() {
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
void LogicImpl::setTypes(const QVector<int> &types)
{
    m_types = types;
}

void LogicImpl::setMaxMovesCount(int maxMovesCount)
{
    m_maxMovesCount = maxMovesCount;
}

void LogicImpl::setMinScore(int minScore)
{
    m_minScore = minScore;
}

void LogicImpl::setElementScore(int elementScore)
{
    m_elementScore = elementScore;
}



///////////////////////////////////////////////////////////////////////////////

QSharedPointer<Tile> LogicImpl::item(int index) const {
    return m_dataList[index];
}

int LogicImpl::indexOfItem(const Tile *item) const {
    for (int i = 0; i < m_dataList.size(); i++) {
        if (m_dataList[i] == item) {
            return i;
        }
    }

    return -1;
}

void LogicImpl::eraseItem(int index) {
    m_dataList.erase(m_dataList.begin() + index);
}

void LogicImpl::insertItem(int index) {
    m_dataList.insert(m_dataList.begin() + index, QSharedPointer<Tile>(new Tile(getRandType())));
}

void LogicImpl::moveTile(int index) {
    if (!m_packList.empty())
        return;

    if (gameResult() != NotFinished)
        return;

    Cell curr_cell(index);

    if (!m_dragedCell.valid()) {
        m_dragedCell = Cell(index);

       emit startScaleTimer();

    }
    else {
        if (able_to_move(curr_cell)) {
            TilesModel::Instance()->changeScale(m_dataList[m_dragedCell.index()], 1);
            emit stopScaleTimer();

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
            }
            else {
                Package pack;

                pack.push(QSharedPointer<Command>(new SwapCommand(curr_cell, m_dragedCell)));
                m_packList.enqueue(pack);
                pack.clear();

                pack.push(QSharedPointer<Command>(new SwapCommand(curr_cell, m_dragedCell)));
                m_packList.enqueue(pack);

                TilesModel::Instance()->execNextPackage();

            }

            m_dragedCell = Cell();

        }
        else {
            TilesModel::Instance()->changeScale(m_dataList[m_dragedCell.index()], 1);
            m_dragedCell = Cell(index);
        }
    }

}

void LogicImpl::execNextPackage() {
    if (m_packList.empty()) {
        createPackages();
        return;
    }

    Package pack = m_packList.dequeue();
    pack.exec();
    setPackDelay(pack.duration());

    emit statusChanged();
    emit startPackTimer();

    if (m_packList.empty()) {
        switch (gameResult()) {
        case Win:
            emit showMessage("Congratulation, you win!!");
            break;
        case Lose:
            emit showMessage("Sorry, but you lose, try again!!");
            break;
        default:
            break;
        }
    }
}

void LogicImpl::provideScaleAnimation()
{
    if (!m_dragedCell.valid())
        return;

    QSharedPointer<Tile> tile =  m_dataList[m_dragedCell.index()];
    float scale = tile->scale();
    if (scale > 1) {
        scale = scale / 1.2;
    }
    else {
        scale = scale * 1.2;
    }

    TilesModel::Instance()->changeScale(tile, scale);
}

void LogicImpl::swapItems(const int index1, const int index2) {
    qSwap(m_dataList[index1], m_dataList[index2]);
}

int LogicImpl::modelSize() const {
//    return m_width * m_height;
    return m_dataList.size();
}

int LogicImpl::width() const {
    return m_width;
}

void LogicImpl::setWidth(int width) {
    m_width = width;
    emit widthChanged();
}
int LogicImpl::height() const {
    return m_height;
}

void LogicImpl::setHeight(int height) {
    m_height = height;
    emit heightChanged();
}

int LogicImpl::packDelay() const {
    return m_packDelay;
}

void LogicImpl::setPackDelay(int packDelay) {
    m_packDelay = packDelay;
    emit packDelayChanged();
}

QString LogicImpl::status()
{
    return QString(" score: %1; moves: %2 / %3").arg(m_score).arg(m_movesCount).arg(m_maxMovesCount);
}
