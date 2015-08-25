#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>

#include <vector>

#include "common_types.h"
#include "tile.h"
#include "cell.h"



class TilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int width READ getWidth WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ getHeight WRITE setHeight NOTIFY heightChanged)

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


signals:
    void widthChanged();
    void heightChanged();


public slots:
    void someSlot();
    void moveTile(int);

    int getWidth();
    void setWidth(const int);

    int getHeight();
    void setHeight(const int);


private:
    TilesModel();
    TilesModel(const TilesModel&);
    TilesModel& operator=(const TilesModel&);

    // for game logic
    bool able_to_move(Cell);
    std::vector<Cell> cellsToMove(Cell);

    std::vector<std::vector<Tile *> > findMatches() const;
    void removeMatches();

    void swapCells(const int, const int);
    void swapCells(const Cell&, const Cell&);

    QString getRandType();

private:
    std::vector<Tile *> data_list_;

    int width_;
    int height_;
    int element_score_;
    int min_scores_;
    int max_moves_;

    Cell draged_cell_;
};

#endif // TILESMODEL_H
