#include "tilesmodel.h"

#include <cmath>



TilesModel::TilesModel(QObject *parent) :
    QAbstractListModel(parent),
    width_(10),
    height_(15)
{
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
