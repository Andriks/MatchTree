#ifndef TILE_H
#define TILE_H

#include <QString>


class Tile
{
public:
    explicit Tile(QString type = "", QString color = "", float opacity = 0);

    QString type() const;
    void setType(const QString &type);

    QString color() const;
    void setColor(const QString &color);

    float opacity() const;
    void setOpacity(const float &opacity);

    int index() const;
    void setIndex(int index);

    QString text() const;
    void setText(const QString &text);

private:
    QString type_;
    QString color_;
    float opacity_;

    QString text_; // tmp for testing

};

#endif
