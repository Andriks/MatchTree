TEMPLATE = app

QT += qml quick widgets

SOURCES += \
    tile.cpp \
    tilesmodel.cpp \
    cell.cpp \
    command.cpp \
    package.cpp \
    jsonparser.cpp \
    mainMT.cpp

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
    jsonparser.h
