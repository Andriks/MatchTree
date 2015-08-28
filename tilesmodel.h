#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>

#include <vector>
#include <queue>

#include "tile.h"
#include "cell.h"
#include "package.h"



class TilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int execPackCnt READ execPackCnt WRITE setExecPackCnt NOTIFY execPackCntChanged)

    ///////////////////////////////////////////////////////
    enum TileElemRoles {
        TypeRole = Qt::UserRole + 1,
        ColorRole,
        OpacityRole,
        TextRole
    };

public:
    ~TilesModel();
    static TilesModel *Instance();
    void generate();

signals:
    void widthChanged();
    void heightChanged();
    void execPackCntChanged();


public slots:
    void moveTile(int index);

    void execNextPackage();
    void createPackages();       //tmp move to slots for testing

    void someSlot();

    // setters / getters for qml engine
    int width();
    void setWidth(const int width);

    int height();
    void setHeight(const int height);

    int execPackCnt() const;
    void setExecPackCnt(int exec_pack_cnt);

public:
    // interface for Command
    void swapCells(const int from, const int to);
    void swapCells(const Cell &from, const Cell &to);

    void changeOpacity(Tile *target, const float opacity);

    void createItem(int index);
    void refreshItem(int index);

    Tile *item(int index);
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

    std::vector<int> getTypes() const;
    void setTypes(const std::vector<int> &types);

    static bool getInitialised();
    static void setInitialised(bool initialised);

private:
    // for game logic
    bool able_to_move(Cell);
    std::vector<Cell> cellsToMove(Cell cell);

    std::vector<std::vector<Tile *> > findMatches() const;
    bool checkForRepeating(Tile *tile, std::vector<std::vector<Tile *> > conteiner) const;
    bool matchesExisting();


    // for singleton
    TilesModel();
    TilesModel(const TilesModel&);
    TilesModel& operator=(const TilesModel&);

private:
    std::vector<Tile *> data_list_;
    std::queue<Package> pack_list_;
    std::vector<int> types_;

    int width_;
    int height_;
    int element_score_;
    int min_score_;
    int max_moves_;

    Cell draged_cell_;
    int exec_pack_cnt_;

    static bool initialised_;
};

#endif // TILESMODEL_H
