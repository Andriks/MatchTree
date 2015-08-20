#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>

#include <vector>

#include "common_types.h"
#include "tile.h"

class TilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int width READ getWidth WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ getHeight WRITE setHeight NOTIFY heightChanged)

    ///////////////////////////////////////////////////////
    enum TileElemRoles {
        ColorRole = Qt::UserRole + 1,
        TypeRole
    };

public:
    TilesModel(QObject *parent = 0);

    // for QML Engine
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;


signals:
    void widthChanged();
    void heightChanged();

public slots:
    int getWidth();
    void setWidth(const int);

    int getHeight();
    void setHeight(const int);


private:
    std::vector<Tile *> data_list_;
    int width_;
    int height_;
};

#endif // TILESMODEL_H
