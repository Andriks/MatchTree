#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QVector>
#include <QString>

class JsonParser
{
public:
    JsonParser(QString file_pas);
    void fillParamsIntoModel();

private:
    QVector<int> types_;

    int width_;
    int height_;
    int element_score_;
    int min_score_;
    int max_moves_;

};

#endif // JSONPARSER_H
