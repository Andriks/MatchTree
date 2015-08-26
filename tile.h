#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QString>


class Tile : public QObject
{
    Q_OBJECT
public:
    explicit Tile(QObject *parent = 0, QString type = "", QString color = "", int index = 0, float opacity = 0);

    QString type() const;
    void setType(const QString &type);

    QString color() const;
    void setColor(const QString &color);

    float opacity() const;
    void setOpacity(const float &opacity);

    int index() const;
    void setIndex(int index);

signals:

public slots:

private:
    QString type_;
    QString color_;
    float opacity_;
    int index_;


};

#endif
