#include "tile.h"

Tile::Tile(QObject *parent, QString type, QString color, int opacity) :
    QObject(parent),
    type_(type),
    color_(color),
    opacity_(opacity)
{

}

QString Tile::getType() const
{
    return type_;
}

void Tile::setType(const QString &type)
{
    type_ = type;
}

QString Tile::getColor() const
{
    return color_;
}

void Tile::setColor(const QString &color)
{
    color_ = color;
}

float Tile::getOpacity() const
{
    return opacity_;
}

void Tile::setOpacity(const float &opacity)
{
    opacity_ = opacity;
}






