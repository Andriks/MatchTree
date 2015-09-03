#include "tile.h"
#include "tilesmodel.h"
//#include <QAbstractItemModel>

Tile::Tile(QString type) {
    setDefault(type);
}

void Tile::setDefault(QString type) {
    m_type = type;

    if (m_type == "") {
        m_valid = false;
        m_opacity = 0;
        m_scale = 1;
    } else {
        m_valid = true;
        m_opacity = 1;
        m_scale = 1;
    }
}

QString Tile::type() const {
    return m_type;
}

float Tile::opacity() const {
    return m_opacity;
}

void Tile::setOpacity(const float &opacity) {
    m_opacity = opacity;
}

int Tile::index() const {
    return TilesModel::Instance()->indexOfItem(this);
}

bool Tile::valid() const {
    return m_valid;
}

void Tile::setValid(bool valid) {
    m_valid = valid;
}
float Tile::scale() const {
    return m_scale;
}

void Tile::setScale(float scale) {
    m_scale = scale;
}

