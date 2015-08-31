#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QTimer>

#include <QVector>
#include <QQueue>

#include "tile.h"
#include "cell.h"
#include "package.h"



class TilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int execPackCnt READ execPackCnt WRITE setExecPackCnt NOTIFY execPackCntChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
//    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)

    ///////////////////////////////////////////////////////
    enum TileElemRoles {
        TypeRole = Qt::UserRole + 1,
        ColorRole,
        OpacityRole,
        TextRole,
        ScaleRole
    };

public:
    static TilesModel *Instance();
    void generate();

private:
    bool leadsToMatch(QSharedPointer<Tile> new_tile);

signals:
    void widthChanged();
    void heightChanged();
    void execPackCntChanged();
    void statusChanged();
    void scaleChanged();


public slots:
    void moveTile(int index);
    void provideScaleAnimation();

    void execNextPackage();
    void createPackages();       //tmp move to slots for testing

    void someSlot(int index);

    // setters / getters for qml engine
    int width();
    void setWidth(const int width);

    int height();
    void setHeight(const int height);

    int execPackCnt() const;
    void setExecPackCnt(int exec_pack_cnt);

    QString status();

public:
    // interface for Command
    void swapCells(const int from, const int to);
    void swapCells(const Cell &from, const Cell &to);

    void changeOpacity(QSharedPointer<Tile> target, const float opacity);
    void changeScale(QSharedPointer<Tile> target, const float scale);

    void createItem(int index);
    void refreshItem(int index);

    QSharedPointer<Tile> item(int index);
    QString getRandType();


    // interface for Tile
    int indexOfItem(const Tile *item) const;


    // for QML Engine
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;


    // setters / getters
    int getElement_score() const;
    void setElement_score(int element_score);

    int getMin_score() const;
    void setMin_score(int min_score);

    int getMax_moves() const;
    void setMax_moves(int max_moves);

    QVector<int> getTypes() const;
    void setTypes(const QVector<int> &types);

    static bool getInitialised();
    static void setInitialised(bool initialised);

//    float scale() const;
//    void setScale(float draged_cell_scale);

    QObject *getRoot() const;
    void setRoot(QObject *root);

private:
    // for game logic
    bool able_to_move(Cell);
    QVector<Cell> cellsToMove(Cell cell);

    QVector<QVector<QSharedPointer<Tile> > > findMatches() const;
    bool checkForRepeating(QSharedPointer<Tile> tile, QVector<QVector<QSharedPointer<Tile> > > conteiner) const;
    bool matchesExisting();


    // for singleton
    TilesModel();
    TilesModel(const TilesModel&);
    TilesModel& operator=(const TilesModel&);

private:
    QObject *root_;

    QVector<QSharedPointer<Tile> > data_list_;
    QQueue<Package> pack_list_;
    QVector<int> types_;

    int width_;
    int height_;
    int element_score_;
    int min_score_;
    int max_moves_;

    int moves_cnt_;
    int score_;

    int exec_pack_cnt_;

    Cell draged_cell_;
//    float draged_cell_scale_;
    QTimer draged_cell_timer_;

    static bool initialised_;
};

#endif // TILESMODEL_H
