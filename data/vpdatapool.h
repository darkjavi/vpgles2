#ifndef VPDATAPOOL_H
#define VPDATAPOOL_H

#include <QObject>
#include <QVector>
#include <QThread>
#include <QDebug>

#include "bthelper.h"
#include "vpdataset.h"
//#include "vpparsermindflex.h"
//#include "vpparsertorque.h"

class vpDataPool : public QObject
{
    Q_OBJECT
public:
    explicit vpDataPool(QObject *parent = 0);
    QVector<vpDataSet*> dataSets()  {return m_dataPool;}
    int count()                     {return m_dataPool.count();}
    vpDataSet* addDatasetCSV(QString filename);
    vpDataSet* addDatasetNMEA(QString filename);
    vpDataSet* addDatasetTorqueLog(QString filename);
    vpDataSet* addDatasetMindflexCSV(QString filename);
    vpDataSet* addDatasetMindflexLive(QString macaddr);
protected:
    void add_dataSet(vpDataSet* dataSet);
    QVector<vpDataSet*> m_dataPool;
    QThread m_thread;
    btHelper*   m_btHelper;
signals:
    void updated();
    void newDataSet(vpDataSet* dataset);
private slots:
    void datasetDeleted(QObject* obj);
public slots:
    void addEmptyDataSet();
    void importCSV(QString filename);
    void importNMEA(QString filename);
    void importTorqueLog(QString filename);
    void importMindflexCSV(QString filename);
    void recursiveImportNMEA(QString basepath);

    void connectMindflex(QString macaddr);
    void setBtHelper(btHelper* h){m_btHelper= h;}

};
#endif // VPDATAPOOL_H
