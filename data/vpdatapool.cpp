#include "vpdatapool.h"
#include <QDir>
#include <QFileInfo>

vpDataPool::vpDataPool(QObject *parent) :
    QObject(parent)
{
    m_btHelper=0;
    this->moveToThread(&m_thread);
    m_thread.start();
}

void vpDataPool::add_dataSet(vpDataSet *dataSet)
{
    m_dataPool.append(dataSet);
    connect(dataSet,SIGNAL(destroyed(QObject*)),this,SLOT(datasetDeleted(QObject*)));
    emit newDataSet(dataSet);
    emit updated();
}

void    vpDataPool::addEmptyDataSet()
{
    add_dataSet(new vpDataSet(this));
}

vpDataSet* vpDataPool::addDatasetCSV(QString filename)
{
    vpDataSet* d = new vpDataSet(this);
    //d->importCSV(filename);
    add_dataSet(d);
    return d;
}

vpDataSet* vpDataPool::addDatasetNMEA(QString filename)
{
    vpDataSet* d = new vpDataSet(this);
    d->dataSource()->importNMEA(filename);
    add_dataSet(d);
    return d;
}

vpDataSet* vpDataPool::addDatasetTorqueLog(QString filename)
{
    vpDataSet* d = new vpDataSet(this);
    //d->importTorqueLog(filename);
    add_dataSet(d);
    return d;
}

vpDataSet* vpDataPool::addDatasetMindflexCSV(QString filename)
{
    vpDataSet* d = new vpDataSet(this);
    //d->importMindflexCSV(filename);
    add_dataSet(d);
    return d;
}

vpDataSet* vpDataPool::addDatasetMindflexLive(QString macaddr)
{
    vpDataSet* d = new vpDataSet(this);
    add_dataSet(d);
    if(!m_btHelper)
    {
        qDebug()<<"[WARN]vpDataPool::connectMindflex-> Set me a btHelper first!";
        return d;
    }

    //d->setBtHelper(m_btHelper);
    //d->connectMindflex(macaddr);//invokemethod??
    return d;
}

void vpDataPool::importCSV(QString filename)
{
    addDatasetCSV(filename);
}

void vpDataPool::importNMEA(QString filename)
{
    addDatasetNMEA(filename);
}

void vpDataPool::importTorqueLog(QString filename)
{
    addDatasetTorqueLog(filename);
}

void vpDataPool::importMindflexCSV(QString filename)
{
    addDatasetMindflexCSV(filename);
}

void vpDataPool::recursiveImportNMEA(QString basepath)
{
    QDir dir(basepath);
    QFileInfoList list = dir.entryInfoList();
    for (int iList=0;iList<list.count();iList++)
    {
        QFileInfo info = list[iList];

        QString sFilePath = info.filePath();
        if (info.isDir())
        {
            // recursive
            if (info.fileName()!=".." && info.fileName()!=".")
            {
                recursiveImportNMEA(sFilePath);
            }
        }
        else
        {
            if (info.suffix().contains("nmea"))
            {

                addDatasetNMEA(info.absoluteFilePath());
            }
        }
    }
}




void vpDataPool::connectMindflex(QString macaddr)
{
    addDatasetMindflexLive(macaddr);
}

void vpDataPool::datasetDeleted(QObject *obj)
{
    vpDataSet* ds =(vpDataSet*) obj;
    if(m_dataPool.contains(ds))
    {
        m_dataPool.remove(m_dataPool.indexOf(ds));
    }
    else
    {
        qDebug()<<"[ERROR]vpDataPool::datasetDeleted-> Cannot find"<<ds<<" for removal!";
    }
    emit updated();
}
