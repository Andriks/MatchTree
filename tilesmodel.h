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

    ///////////////////////////////////////////////////////
    enum TileElemRoles {
        TypeRole = Qt::UserRole + 1,
        ColorRole,
        OpacityRole
    };

public:
    static TilesModel *Instance();

    // for QML Engine
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    // interface for Command
    void swapCells(const int from, const int to);
    void swapCells(const Cell &from, const Cell &to);
    void changeOpacity(const Cell &target, const float opacity);


signals:
    void widthChanged();
    void heightChanged();


public slots:
    void someSlot();
    void moveTile(int);
    void execNextPackage();
    void removeMatches();       //tmp move to slots for testing

    int width();
    void setWidth(const int);

    int height();
    void setHeight(const int);


private:
    TilesModel();
    TilesModel(const TilesModel&);
    TilesModel& operator=(const TilesModel&);

    // for game logic
    bool able_to_move(Cell);
    std::vector<Cell> cellsToMove(Cell);

    std::vector<std::vector<Tile *> > findMatches() const;


    QString getRandType();


private:
    std::vector<Tile *> data_list_;
    std::queue<Package> pack_list_;

    int width_;
    int height_;
    int element_score_;
    int min_scores_;
    int max_moves_;

    Cell draged_cell_;
};

#endif // TILESMODEL_H
