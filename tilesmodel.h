#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>

#include "tile.h"
#include "cell.h"
#include "package.h"
#include "logicimpl.h"
#include "durationsconf.h"



class TilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(LogicImpl *config READ config NOTIFY configChanged)
    Q_PROPERTY(DurationsConf *durations READ durations NOTIFY durationsChanged)

    ///////////////////////////////////////////////////////
    enum TileElemRoles {
        TypeRole = Qt::UserRole + 1,
        OpacityRole,
        ScaleRole
    };

    // for singleton
    TilesModel();
    TilesModel(const TilesModel&);
    TilesModel& operator=(const TilesModel&);

public:
    static TilesModel *Instance();

signals:
    void configChanged();
    void durationsChanged();

public slots:
    LogicImpl *config() const;
    DurationsConf *durations() const;

    // start generating items (without matches)
    void newGame();

    void moveTile(int index);
    void provideScaleAnimation();

    // for packeges executing
    void execNextPackage();


public:
    /***************************/
    /***interface for Command***/
    /***************************/

    void swapCells(const int from, const int to);
    void swapCells(const Cell &from, const Cell &to);

    void changeOpacity(QSharedPointer<Tile> target, const float opacity);
    void changeScale(QSharedPointer<Tile> target, const float scale);

    void createItem(int index);
    void refreshItem(QSharedPointer<Tile> target);

    QSharedPointer<Tile> item(int index);


    /***************************/
    /*****interface for Tile****/
    /***************************/

    int indexOfItem(const Tile *item) const;


    // for QML Engine
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;



public:
    int width();
    int height();

private:
    LogicImpl *m_logicImpl;
    DurationsConf *m_durations;

};

#endif // TILESMODEL_H
