#include "tilesmodel.h"
#include "command.h"


TilesModel::TilesModel() :
    m_logicImpl(NULL),
    m_durations(NULL)
{
    m_logicImpl = new LogicImpl();
    m_durations = new DurationsConf();
}

TilesModel *TilesModel::Instance() {
    static TilesModel theSingleInstance;
    return &theSingleInstance;
}

LogicImpl *TilesModel::config() const
{
    return m_logicImpl;
}

DurationsConf *TilesModel::durations() const
{
    return m_durations;
}

void TilesModel::newGame() {
    if (m_logicImpl->packListSize())
        return;

    beginInsertRows(QModelIndex(), 0, m_logicImpl->modelSize() - 1);
    m_logicImpl->newGame();
    endInsertRows();

    emit configChanged();
}

int TilesModel::indexOfItem(const Tile *item) const {
    return m_logicImpl->indexOfItem(item);
}

int TilesModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent)
    return m_logicImpl->modelSize();
}

QVariant TilesModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_logicImpl->modelSize())
        return QVariant();

    const QSharedPointer<Tile> tile = m_logicImpl->item(index.row());

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

void TilesModel::execNextPackage() {
    m_logicImpl->execNextPackage();
}

void TilesModel::swapCells(const int from, const int to) {
    if (from == to)
        return;

    if (std::abs(from - to) > 1) {
        int min = std::min(from, to);
        int max = std::max(from, to);

        beginMoveRows(QModelIndex(), max, max, QModelIndex(), min);
        endMoveRows();

        beginMoveRows(QModelIndex(), min+1, min+1, QModelIndex(), max+1);
        endMoveRows();
    }
    else {
        int add_num = 0;

        if (from < to)
            add_num = 1;

        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + add_num);
        endMoveRows();

    }

    m_logicImpl->swapItems(from, to);
}

void TilesModel::swapCells(const Cell &from, const Cell &to) {
    swapCells(from.index(), to.index());
}

void TilesModel::changeOpacity(QSharedPointer<Tile> target, const float opacity) {
    target->setOpacity(opacity);

    emit dataChanged(createIndex(target->index(), 0), createIndex(target->index(), 0));
}

void TilesModel::changeScale(QSharedPointer<Tile> target, const float scale) {
    target->setScale(scale);

    emit dataChanged(createIndex(target->index(), 0), createIndex(target->index(), 0));
}

void TilesModel::createItem(int index) {
    beginRemoveRows(QModelIndex(), index, index);
    m_logicImpl->eraseItem(index);
    endRemoveRows();

    beginInsertRows(QModelIndex(), index, index);
    m_logicImpl->insertItem(index);
    endInsertRows();
}

void TilesModel::refreshItem(QSharedPointer<Tile> target) {
    beginResetModel();
    target->setDefault(m_logicImpl->getRandType());
    endResetModel();
}

QSharedPointer<Tile> TilesModel::item(int index) {
    return m_logicImpl->item(index);
}

void TilesModel::moveTile(int index) {
    m_logicImpl->moveTile(index);
}

void TilesModel::provideScaleAnimation() {
    m_logicImpl->provideScaleAnimation();

}

int TilesModel::width() {
    return m_logicImpl->width();
}

int TilesModel::height() {
    return m_logicImpl->height();
}

