#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "logicimpl.h"
#include "durationsconf.h"

#include <QString>


class JsonParser
{
public:
    JsonParser();

    void parse_config(QString file_pas, LogicImpl *target);
    void parse_durations(QString file_pas, DurationsConf *target);
};

#endif // JSONPARSER_H
