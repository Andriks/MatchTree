TEMPLATE = app

QT += qml quick widgets

SOURCES += \
    tile.cpp \
    tilesmodel.cpp \
    cell.cpp \
    command.cpp \
    package.cpp \
    mainMT.cpp \
    logicimpl.cpp \
    jsonparser.cpp \
    durationsconf.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    tile.h \
    tilesmodel.h \
    cell.h \
    command.h \
    package.h \
    logicimpl.h \
    jsonparser.h \
    durationsconf.h
