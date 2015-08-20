#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>

#include <vector>
#include <exception>

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

    ///////////////////////////////////////////////////////
    class Cell
    {
    public:
        Cell();
        Cell(int r, int c);
        Cell(const TilesModel::Cell &cell);
        Cell(int index);

        bool operator ==(const Cell);

        bool valid() const;
        int getIndex() const;

        static void setStaticParams(int width, int height);

    //private:
        int row;
        int col;

    private:
        static int width_;
        static int height_;
    };


public:
    static TilesModel *Instance();

    // for QML Engine
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    // for game logic
    bool able_to_move(Cell);
    std::vector<Cell> cellsToMove(Cell);

    std::vector< std::vector<Cell> > findMatches() const;

    void swapCells(const int, const int);
    void swapCells(const Cell&, const Cell&);



signals:
    void widthChanged();
    void heightChanged();

public slots:
    void moveTile(int);

    int getWidth();
    void setWidth(const int);

    int getHeight();
    void setHeight(const int);

private:
    TilesModel();
    TilesModel(const TilesModel&);
    TilesModel& operator=(const TilesModel&);


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
