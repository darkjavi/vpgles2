#include "vpparserremoteparser.h"

vpParserRemoteParser::vpParserRemoteParser(vpData* data, QObject *parent) : vpParser(data,parent)
{
    m_source=data->id();
}
