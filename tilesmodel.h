#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>

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
    Q_PROPERTY(int packDelay READ packDelay WRITE setPackDelay NOTIFY packDelayChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

    ///////////////////////////////////////////////////////
    enum TileElemRoles {
        TypeRole = Qt::UserRole + 1,
        OpacityRole,
        ScaleRole
    };

    ///////////////////////////////////////////////////////
    enum GameResult {
        Win = Qt::UserRole + 1,
        Lose,
        NotFinished
    };

    // for singleton
    TilesModel();
    TilesModel(const TilesModel&);
    TilesModel& operator=(const TilesModel&);

public:
    static TilesModel *Instance();
    // start generating items (without matches)
    void generate();


private:
    // check for generate()
    bool leadsToMatch(QSharedPointer<Tile> new_tile);


public slots:
    /*********************/
    /********SLOTS********/
    /*********************/

    void moveTile(int index);
    void provideScaleAnimation();

    void execNextPackage();
    void createPackages();       //tmp move to slots for testing


public:
    /***************************/
    /***interface for Command***/
    /***************************/

    void swapCells(const int from, const int to);
    void swapCells(const Cell &from, const Cell &to);

    void changeOpacity(QSharedPointer<Tile> target, const float opacity);
    void changeScale(QSharedPointer<Tile> target, const float scale);

    void createItem(int index);
    void refreshItem(int index);

    QSharedPointer<Tile> item(int index);
    QString getRandType();


    /***************************/
    /*****interface for Tile****/
    /***************************/

    int indexOfItem(const Tile *item) const;


    // for QML Engine
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;


private:
    /***************************/
    /*******for game logic******/
    /***************************/

    bool able_to_move(Cell);
    QVector<Cell> cellsToMove(Cell cell);

    // returns vector of hor / vert matches
    QVector<QVector<QSharedPointer<Tile> > > findMatches() const;
    // checks for repeating tiles in matches
    bool checkForRepeating(QSharedPointer<Tile> tile, QVector<QVector<QSharedPointer<Tile> > > conteiner) const;
    bool matchesExisting();

    GameResult gameResult();


public slots:
    // setters / getters for qml engine
    int width();
    void setWidth(const int width);

    int height();
    void setHeight(const int height);

    int packDelay() const;
    void setPackDelay(int packDelay);

    QString status();


public:
    // setters / getters
    int elementScore() const;
    void setElementScore(int element_score);

    int minScore() const;
    void setMinScore(int minScore);

    int maxMoves() const;
    void setMaxMoves(int maxMoves);

    QVector<int> getTypes() const;
    void setTypes(const QVector<int> &types);

    static bool getInitialised();
    static void setInitialised(bool initialised);

    QObject *getRoot() const;
    void setRoot(QObject *root);


signals:
    void widthChanged();
    void heightChanged();
    void packDelayChanged();
    void statusChanged();
    void scaleChanged();


private:
    QObject *m_root;

    QVector<QSharedPointer<Tile> > m_dataList;
    QQueue<Package> m_packList;
    QVector<int> m_types;

    int m_width;
    int m_height;
    int m_elementScore;
    int m_minScore;
    int m_maxMovesCount;

    int m_movesCount;
    int m_score;

    Cell m_dragedCell;

    int m_packDelay;

    static bool m_initialised;
};

#endif // TILESMODEL_H
