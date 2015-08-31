#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QList>
#include <QObject>

#include <QtQml>

#include "tilesmodel.h"
#include "jsonparser.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    JsonParser parser("../MatchTree/input.json");
    parser.fillParamsIntoModel();

    QQmlApplicationEngine engine;

    TilesModel *model = TilesModel::Instance();
    engine.rootContext()->setContextProperty("dataModel", model);

    engine.load(QUrl(QStringLiteral("qrc:/mainMT.qml")));

    model->setRoot(engine.rootObjects()[0]);

    return app.exec();
}
