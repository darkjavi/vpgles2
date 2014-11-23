#ifndef VPPARSERREMOTEPARSER_H
#define VPPARSERREMOTEPARSER_H
#include "vpparser.h"

class vpParserRemoteParser : public vpParser
{
    Q_OBJECT
public:
    vpParserRemoteParser(vpData* data, QObject* parent=0);
    bool    openSource(QString){return true;}
public slots:
    void    closeSource(){;}
};

#endif // VPPARSERREMOTEPARSER_H
