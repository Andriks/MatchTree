#include "tile.h"

Tile::Tile(QObject *parent, QString color, QString type) :
    QObject(parent),
    color_(color),
    type_(type)
{

}

QString Tile::getColor() const
{
    return color_;
}

QString Tile::getType() const
{
    return type_;
}

