#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QList>
#include <QObject>

#include <QtQml>

#include "tilesmodel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    TilesModel *model = TilesModel::Instance();
    model->newGame();
    engine.rootContext()->setContextProperty("dataModel", model);
    engine.load(QUrl(QStringLiteral("qrc:/mainMT.qml")));


    return app.exec();
}
