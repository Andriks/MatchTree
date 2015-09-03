#include "command.h"
#include "tilesmodel.h"

#include "QDebug"

Command::Command()
{

}


//////////////////////////////////////////////////////////////
SwapCommand::SwapCommand(QSharedPointer<Tile> from, QSharedPointer<Tile> to) :
    m_from(from),
    m_to(to)
{

}

SwapCommand::SwapCommand(const Cell &from, const Cell &to) :
    m_from(NULL),
    m_to(NULL)
{
    m_from = TilesModel::Instance()->item(from.index());
    m_to = TilesModel::Instance()->item(to.index());
}

//////////////////////////////////////////////////////////////
void SwapCommand::exec() {
//    int duration = TilesModel::Instance()->durations()->swap();
//    TilesModel::Instance()->durations()->setMove(duration);

    TilesModel::Instance()->swapCells(m_from->index(), m_to->index());
}


int SwapCommand::duration() {
    int duration = TilesModel::Instance()->durations()->swap();

    TilesModel::Instance()->durations()->setMove(duration);
    return duration;
}

QSharedPointer<Tile> SwapCommand::from() const {
    return m_from;
}

void SwapCommand::setFrom(QSharedPointer<Tile> from) {
    m_from = from;
}

QSharedPointer<Tile> SwapCommand::to() const {
    return m_to;
}

void SwapCommand::setTo(QSharedPointer<Tile> to) {
    m_to = to;
}


//////////////////////////////////////////////////////////////
MoveUpCommand::MoveUpCommand(QSharedPointer<Tile> target) :
    m_target(target)
{

}

void MoveUpCommand::exec() {
//    int duration = TilesModel::Instance()->durations()->moveUp();
//    TilesModel::Instance()->durations()->setMove(duration);

    Cell cell = Cell(m_target->index());

    while (cell.upper().valid()) {
        TilesModel::Instance()->swapCells(cell, cell.upper());

        cell = cell.upper();
    }
}

int MoveUpCommand::duration() {
    int duration = TilesModel::Instance()->durations()->moveUp();

    TilesModel::Instance()->durations()->setMove(duration);
    return duration;
}

QSharedPointer<Tile> MoveUpCommand::target() const {
    return m_target;
}

void MoveUpCommand::setTarget(QSharedPointer<Tile> target) {
    m_target = target;
}


//////////////////////////////////////////////////////////////
OpacityCommand::OpacityCommand(QSharedPointer<Tile> target, float opacity) :
    m_target(target),
    m_opacity(opacity)
{

}

void OpacityCommand::exec() {
    m_target->setValid(false);
    TilesModel::Instance()->changeOpacity(m_target, m_opacity);
}

int OpacityCommand::duration() {
    return TilesModel::Instance()->durations()->opacity();
}

QSharedPointer<Tile> OpacityCommand::target() const {
    return m_target;
}

void OpacityCommand::setTarget(QSharedPointer<Tile> target) {
    m_target = target;
}

float OpacityCommand::opacity() const {
    return m_opacity;
}

void OpacityCommand::setOpacity(float opacity) {
    m_opacity = opacity;
}


//////////////////////////////////////////////////////////////
ScaleCommand::ScaleCommand(QSharedPointer<Tile> target, float scale) :
    m_target(target),
    m_scale(scale)
{

}

void ScaleCommand::exec() {
    m_target->setValid(false);
    TilesModel::Instance()->changeScale(m_target, m_scale);
}

int ScaleCommand::duration() {
    return TilesModel::Instance()->durations()->scale();
}

QSharedPointer<Tile> ScaleCommand::target() const {
    return m_target;
}

void ScaleCommand::setTarget(QSharedPointer<Tile> target) {
    m_target = target;
}
float ScaleCommand::scale() const {
    return m_scale;
}

void ScaleCommand::setScale(float scale) {
    m_scale = scale;
}


//////////////////////////////////////////////////////////////
CreateCommand::CreateCommand(QSharedPointer<Tile> target) :
    m_target(target)
{

}

void CreateCommand::exec() {
    TilesModel::Instance()->createItem(m_target->index());
}

int CreateCommand::duration() {
    return TilesModel::Instance()->durations()->create();
}

QSharedPointer<Tile> CreateCommand::target() const {
    return m_target;
}

void CreateCommand::setTarget(QSharedPointer<Tile> target) {
    m_target = target;
}

//////////////////////////////////////////////////////////////
RefreshCommand::RefreshCommand(QSharedPointer<Tile> target) :
    m_target(target)
{

}

void RefreshCommand::exec() {
    TilesModel::Instance()->refreshItem(m_target);
}

int RefreshCommand::duration() {
    return TilesModel::Instance()->durations()->refresh();
}

QSharedPointer<Tile> RefreshCommand::target() const {
    return m_target;
}

void RefreshCommand::setTarget(QSharedPointer<Tile> target) {
    m_target = target;
}
