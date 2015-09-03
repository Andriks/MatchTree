#ifndef TILE_H
#define TILE_H

#include <QString>


class Tile
{
public:
    explicit Tile(QString type = "");

    void setDefault(QString type = "");

    QString type() const;

    float opacity() const;
    void setOpacity(const float &opacity);

    int index() const;

    bool valid() const;
    void setValid(bool valid);

    float scale() const;
    void setScale(float scale);

private:
    QString m_type;
    bool m_valid;
    float m_opacity;
    float m_scale;

};

#endif
