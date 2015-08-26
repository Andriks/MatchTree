#include "jsonparser.h"
#include "tilesmodel.h"

#include <QDebug>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


JsonParser::JsonParser(QString file_pas)
{
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

    width_ = jsonObject["width"].toInt();
    height_ = jsonObject["height"].toInt();
    element_score_ = jsonObject["elementScore"].toInt();
    min_score_ = jsonObject["minScore"].toInt();
    max_moves_ = jsonObject["maxMoves"].toInt();


    QJsonArray jsonArray = jsonObject["types"].toArray();
    foreach (const QJsonValue & value, jsonArray) {
        int arr_item = value.toInt();
        types_.push_back(arr_item);
    }
}

void JsonParser::fillParamsIntoModel()
{
    TilesModel::Instance()->setWidth(width_);
    TilesModel::Instance()->setHeight(height_);
    TilesModel::Instance()->setElement_score(element_score_);
    TilesModel::Instance()->setMin_score(min_score_);
    TilesModel::Instance()->setMax_moves(max_moves_);
    TilesModel::Instance()->setTypes(types_);

    TilesModel::Instance()->setInitialised(true);

    TilesModel::Instance()->generate();
}

