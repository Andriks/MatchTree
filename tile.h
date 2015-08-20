#ifndef TILE_H
#define TILE_H

#include "common_types.h"

class Tile : public QObject
{
    Q_OBJECT
public:
    explicit Tile(QObject *parent = 0, QString color="", QString type="");

    QString getColor() const;
    QString getType() const;

signals:

public slots:

private:
    QString color_;
    QString type_;

};

#endif
