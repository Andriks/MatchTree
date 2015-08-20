#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QList>
#include <QObject>

#include <QtQml>

#include "controller.h"


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/mt_main.qml")));

    Controller controller(engine.rootObjects()[0]);
    engine.rootContext()->setContextProperty("controller", &controller);

    return app.exec();
}
