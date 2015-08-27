#include "tile.h"
#include "tilesmodel.h"

Tile::Tile(QString type) :
    type_(type)

{
    if (type_ == "") {
        text_ = "X";
        valid_ = false;
        opacity_ = 0;
    } else {
        text_ = "";
        valid_ = true;
        opacity_ = 1;
    }
}

QString Tile::type() const
{
    return type_;
}

void Tile::setType(const QString &type)
{
    type_ = type;
}

QString Tile::color() const
{
    return color_;
}

void Tile::setColor(const QString &color)
{
    color_ = color;
}

float Tile::opacity() const
{
    return opacity_;
}

void Tile::setOpacity(const float &opacity)
{
    opacity_ = opacity;
}
int Tile::index() const
{
    return TilesModel::Instance()->indexOfItem(this);
}

QString Tile::text() const
{
    return text_;
}

void Tile::setText(const QString &text)
{
    text_ = text;
}
bool Tile::valid() const
{
    return valid_;
}

void Tile::setValid(bool valid)
{
    valid_ = valid;
}









