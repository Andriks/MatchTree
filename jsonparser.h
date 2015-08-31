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
    QVector<int> m_types;

    int m_width;
    int m_height;
    int m_elementScore;
    int m_minScore;
    int m_maxMoves;

};

#endif // JSONPARSER_H
