#ifndef LOGICIMPL_H
#define LOGICIMPL_H

#include <QObject>
#include <QVector>
#include <QQueue>
#include <QSharedPointer>

#include "tile.h"
#include "cell.h"
#include "package.h"


class LogicImpl: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int packDelay READ packDelay WRITE setPackDelay NOTIFY packDelayChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

    enum GameResult {
        Win = Qt::UserRole + 1,
        Lose,
        NotFinished
    };

public:
    LogicImpl(QObject *parent = 0);

    void newGame();

    void moveTile(int index);
    void execNextPackage();
    void provideScaleAnimation();

    int modelSize() const;

    QString getRandType();

    QSharedPointer<Tile> item(int index) const;
    int indexOfItem(const Tile *item) const;
    void eraseItem(int index);
    void insertItem(int index);
    void swapItems(const int index1, const int index2);


    void setElementScore(int elementScore);

    void setMinScore(int minScore);

    void setMaxMovesCount(int maxMovesCount);

    void setTypes(const QVector<int> &types);

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
    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

    int packDelay() const;
    void setPackDelay(int packDelay);

    QString status();

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
