#include "durationsconf.h"
#include "jsonparser.h"

DurationsConf::DurationsConf(QObject *parent) :
    QObject(parent),
    m_swap(100),
    m_moveUp(100),
    m_opacity(100),
    m_scale(100),
    m_create(100),
    m_refresh(100)
{
    JsonParser parser;
    parser.parse_durations("../match3/durations.json", this);
}
int DurationsConf::swap() const
{
    return m_swap;
}

void DurationsConf::setSwap(int swap)
{
    m_swap = swap;
    emit swapChanged();
}
int DurationsConf::moveUp() const
{
    return m_moveUp;
}

void DurationsConf::setMoveUp(int moveUp)
{
    m_moveUp = moveUp;
    emit moveUpChanged();
}
int DurationsConf::opacity() const
{
    return m_opacity;
}

void DurationsConf::setOpacity(int opacity)
{
    m_opacity = opacity;
    emit opacityChanged();
}
int DurationsConf::scale() const
{
    return m_scale;
}

void DurationsConf::setScale(int scale)
{
    m_scale = scale;
    emit scaleChanged();
}
int DurationsConf::create() const
{
    return m_create;
}

void DurationsConf::setCreate(int create)
{
    m_create = create;
    emit createChanged();
}
int DurationsConf::refresh() const
{
    return m_refresh;
}

void DurationsConf::setRefresh(int refresh)
{
    m_refresh = refresh;
    emit refreshChanged();
}







