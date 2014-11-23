#ifndef VPPARSERTORQUE_H
#define VPPARSERTORQUE_H

#include "vpparsercsv.h"

class vpParserTorque : public vpParserCSV
{
    Q_OBJECT
public:
    explicit vpParserTorque(QString source,QObject *parent = 0);
protected:

signals:

private slots:

public slots:

};
#endif // VPPARSERTORQUE_H
