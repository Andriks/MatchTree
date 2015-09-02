#ifndef LOGICIMPL_H
#define LOGICIMPL_H

#include <QObject>
#include <QVector>
#include <QQueue>
#include <QSharedPointer>

#include "tile.h"
#include "cell.h"
#include "package.h"


class LogicImpl
{
    enum GameResult {
        Win = Qt::UserRole + 1,
        Lose,
        NotFinished
    };

public:
    LogicImpl();

    void newGame();
    void moveTile(int index);
    void execNextPackage();
    void provideScaleAnimation();

    QString getRandType();

    QSharedPointer<Tile> item(int index) const;
    int indexOfItem(const Tile *item) const;
    void eraseItem(int index);
    void insertItem(int index);
    void swapItems(const int index1, const int index2);


private:
    void parse_params(QString file_pas);

    bool leadsToMatch(QSharedPointer<Tile> new_tile);

    void createPackages();

    bool able_to_move(Cell target_cell);
    QVector<Cell> cellsToMove(Cell cell);

    // returns vector of hor / vert matches
    QVector<QVector<QSharedPointer<Tile> > > findMatches() const;

    // checks for repeating tiles in matches
    bool checkForRepeating(QSharedPointer<Tile> tile, QVector<QVector<QSharedPointer<Tile> > > conteiner) const;
    bool matchesExisting();

    GameResult gameResult();


public:
    /***************************/
    /*****setters / getters*****/
    /***************************/

    int modelSize() const;

    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

    int elementScore() const;
    void setElementScore(int elementScore);

    int minScore() const;
    void setMinScore(int minScore);

    int maxMovesCount() const;
    void setMaxMovesCount(int maxMovesCount);

    int movesCount() const;
    void setMovesCount(int movesCount);

    int score() const;
    void setScore(int score);

    Cell dragedCell() const;
    void setDragedCell(const Cell &dragedCell);

    int packDelay() const;
    void setPackDelay(int packDelay);

    QVector<int> types() const;
    void setTypes(const QVector<int> &types);


private:
    QVector<QSharedPointer<Tile> > m_dataList;
    QQueue<Package> m_packList;

    // game config
    QVector<int> m_types;
    int m_width;
    int m_height;
    int m_elementScore;
    int m_minScore;
    int m_maxMovesCount;

    //game state
    int m_movesCount;
    int m_score;
    Cell m_dragedCell;


    int m_packDelay;

};

#endif // LOGICIMPL_H
