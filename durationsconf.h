#ifndef DURATIONSCONF_H
#define DURATIONSCONF_H

#include <QObject>

class DurationsConf : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int swap READ swap NOTIFY swapChanged)
    Q_PROPERTY(int moveUp READ moveUp NOTIFY moveUpChanged)
    Q_PROPERTY(int move READ move NOTIFY moveChanged)       // depends on what is proccess (swap or moveUp)
    Q_PROPERTY(int opacity READ opacity NOTIFY opacityChanged)
    Q_PROPERTY(int scale READ scale NOTIFY scaleChanged)
    Q_PROPERTY(int create READ create NOTIFY createChanged)
    Q_PROPERTY(int refresh READ refresh NOTIFY refreshChanged)

public:
    explicit DurationsConf(QObject *parent = 0);

signals:
    void swapChanged();
    void moveUpChanged();
    void moveChanged();
    void opacityChanged();
    void scaleChanged();
    void createChanged();
    void refreshChanged();


public slots:
    int swap() const;
    void setSwap(int swap);

    int moveUp() const;
    void setMoveUp(int moveUp);

    int move();
    void setMove(int move);

    int opacity() const;
    void setOpacity(int opacity);

    int scale() const;
    void setScale(int scale);

    int create() const;
    void setCreate(int create);

    int refresh() const;
    void setRefresh(int refresh);

private:
    int m_swap;
    int m_moveUp;
    int m_move;     // depends on what is proccess (swap or moveUp)
    int m_opacity;
    int m_scale;
    int m_create;
    int m_refresh;
};

#endif // DURATIONSCONF_H
