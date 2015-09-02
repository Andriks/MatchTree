#include "jsonparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>


JsonParser::JsonParser()
{
}

void JsonParser::parse_config(QString file_pas, LogicImpl *target)
{
    QFile jsonFile(file_pas);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QJsonParseError parseError;
    QJsonDocument json_doc = QJsonDocument::fromJson(jsonFile.readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        jsonFile.close();
        return;
    }

    if (json_doc.isEmpty()) {
        jsonFile.close();
        return;
    }

    jsonFile.close();

    QJsonObject jsonObject = json_doc.object();

    target->setWidth(jsonObject["width"].toInt());
    target->setHeight(jsonObject["height"].toInt());
    target->setElementScore(jsonObject["elementScore"].toInt());
    target->setMinScore(jsonObject["minScore"].toInt());
    target->setMaxMovesCount(jsonObject["maxMoves"].toInt());


    QVector<int> types;

    QJsonArray jsonArray = jsonObject["types"].toArray();
    foreach (const QJsonValue & value, jsonArray) {
        int arr_item = value.toInt();
        types.push_back(arr_item);
    }

    target->setTypes(types);
}

void JsonParser::parse_durations(QString file_pas, DurationsConf *target)
{
    QFile jsonFile(file_pas);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QJsonParseError parseError;
    QJsonDocument json_doc = QJsonDocument::fromJson(jsonFile.readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        jsonFile.close();
        return;
    }

    if (json_doc.isEmpty()) {
        jsonFile.close();
        return;
    }

    jsonFile.close();

    QJsonObject jsonObject = json_doc.object();

    target->setSwap(jsonObject["swap"].toInt());
    target->setMoveUp(jsonObject["moveUp"].toInt());
    target->setOpacity(jsonObject["opacity"].toInt());
    target->setScale(jsonObject["scale"].toInt());
    target->setCreate(jsonObject["create"].toInt());
    target->setRefresh(jsonObject["refresh"].toInt());
}

