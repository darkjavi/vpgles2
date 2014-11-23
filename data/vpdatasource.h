#ifndef VPDATASOURCE_H
#define VPDATASOURCE_H

#include <QObject>
#include <QThread>

#include "vpdataset.h"
#include "vpparser.h"
#include "vpparsercsv.h"
#include "vpparsernmea.h"
#include "vpparsermindflex.h"
#include "vpparsertorque.h"

class vpDataSet;
class vpDataSource : public QObject
{
    Q_OBJECT
public:
    explicit vpDataSource(vpDataSet* parent);
    bool importCSV(QString filename,QString fieldSep=",",QString lineSep="\n");
    bool importTorque(QString filename);
    bool importNMEA(QString filename);
    bool connectMindFlex(QString macaddr);
    bool connectGPSD(QString host);
protected:
    vpDataSet*  m_dataSet;
    vpParser*   m_parser;
    QThread     m_thread;
signals:

public slots:
    void exportCSV(QString filedest);
    void readAll();
};

#endif // VPDATASOURCE_H
