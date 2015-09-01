#include "jsonparser.h"
#include "tilesmodel.h"

#include <QDebug>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


JsonParser::JsonParser(QString file_pas) {
    QFile jsonFile(file_pas);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "not opened";
        return;
    }

    QJsonParseError parseError;
    QJsonDocument json_doc = QJsonDocument::fromJson(jsonFile.readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << parseError.error;
        jsonFile.close();
        return;
    }

    if (json_doc.isEmpty()) {
        qDebug() << "empty";
        jsonFile.close();
        return;
    }

    jsonFile.close();

    QJsonObject jsonObject = json_doc.object();

    m_width = jsonObject["width"].toInt();
    m_height = jsonObject["height"].toInt();
    m_elementScore = jsonObject["elementScore"].toInt();
    m_minScore = jsonObject["minScore"].toInt();
    m_maxMoves = jsonObject["maxMoves"].toInt();


    QJsonArray jsonArray = jsonObject["types"].toArray();
    foreach (const QJsonValue & value, jsonArray) {
        int arr_item = value.toInt();
        m_types.push_back(arr_item);
    }
}

void JsonParser::fillParamsIntoModel() {
    TilesModel::Instance()->setWidth(m_width);
    TilesModel::Instance()->setHeight(m_height);
    TilesModel::Instance()->setElementScore(m_elementScore);
    TilesModel::Instance()->setMinScore(m_minScore);
    TilesModel::Instance()->setMaxMoves(m_maxMoves);
    TilesModel::Instance()->setTypes(m_types);

    TilesModel::Instance()->setInitialised(true);

    TilesModel::Instance()->newGame();
}

