#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>

#include "tile.h"
#include "cell.h"
#include "package.h"
#include "logicimpl.h"



class TilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int packDelay READ packDelay WRITE setPackDelay NOTIFY packDelayChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

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

public slots:
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


signals:
    void widthChanged();
    void heightChanged();
    void packDelayChanged();
    void statusChanged();
    void scaleChanged();
    void startPackTimer();
    void startScaleTimer();
    void stopScaleTimer();
    void showMessage(QString text);


public slots:
    // setters / getters for qml engine
    int width();
    void setWidth(const int width);

    int height();
    void setHeight(const int height);

    int packDelay() const;
    void setPackDelay(int packDelay);

    QString status();


private:
    LogicImpl m_logicImpl;

};

#endif // TILESMODEL_H
