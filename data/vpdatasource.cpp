#include "vpdatasource.h"
#include <QFileInfo>

vpDataSource::vpDataSource(vpDataSet *parent) :
    QObject() , m_parser(0)
{
    m_dataSet = parent;
    this->moveToThread(&m_thread);
    m_thread.start();
}

bool vpDataSource::importCSV(QString filename,QString fieldSep,QString lineSep)
{
    vpParserCSV* p = new vpParserCSV();
    p->setSeparators(fieldSep,lineSep);
    m_parser = p;
    if(m_parser->openSource(filename))
    {
        QFileInfo fi(filename);
        m_dataSet->initDataset(fi.baseName(),m_parser->fields(),true);
        QMetaObject::invokeMethod(this,"readAll");
        return true;
    }
    return false;
}
bool vpDataSource::importTorque(QString filename)
{

}
bool vpDataSource::importNMEA(QString filename)
{
    vpParserNMEA* p = new vpParserNMEA(filename);
    m_parser = p;
    QFileInfo fi(filename);
    m_dataSet->initDataset(fi.baseName(),m_parser->fields());
    connect(p,SIGNAL(newData(QVector<double>)),m_dataSet,SLOT(appendRowDoubles(QVector<double>)));
    if(m_parser->openSource(filename))
    {
        //qDebug() << "success";
        return true;
    } else {
        qDebug() << "failed"<<filename;
        p->deleteLater();
        m_parser = 0;
        return false;
    }
}
bool vpDataSource::connectMindFlex(QString macaddr)
{

}
bool vpDataSource::connectGPSD(QString host)
{

}

//Slots
void vpDataSource::exportCSV(QString filedest)
{
    if(m_dataSet->isStorageInStrings())
        vpParser::exportCSV(filedest,m_dataSet->getFields(),m_dataSet->dataStrings(),true);
    else
        vpParser::exportCSV(filedest,m_dataSet->getFields(),m_dataSet->dataDoubles(),true);
}

void vpDataSource::readAll()
{

    if(m_parser->isDataInStrings())
    {
        m_dataSet->appendDataStrings(m_parser->readStrings());
    }
    else {
        m_dataSet->appendDataDoubles(m_parser->readDoubles());
    }
    qDebug()<<"readed!";
}
