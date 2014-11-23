#include "vpdataset.h"

vpDataSet::vpDataSet(QObject *parent) :
    QObject(parent),m_source(0),m_data(0),m_btHelper(0)
{
    m_data = new vpDataArrayDoubles("dummy-"+QString::number(qrand() %1000));
    m_data->initFields(QVector<QString>()<<"campo0"<<"campo1"<<"campo2");
    m_data->append(QVector<double>()<<1<<2<<3);
    emit dataUpdated();
}

vpDataSet::~vpDataSet()
{
    clear();
}

bool vpDataSet::isValid()
{
    if(m_data)   return true;
    else         return false;
}

bool vpDataSet::hasParser()
{
    if(m_source) return true;
    else         return false;
}

void vpDataSet::setParser(vpParser *p)
{
    if(m_source)
    {
        disconnect(m_source,SIGNAL(destroyed()),this,SLOT(parserDeleted()));
        m_source->deleteLater();
        emit parserDied();
    }
    m_source = p;
    connect(m_source,SIGNAL(destroyed()),this,SLOT(parserDeleted()));
    connect(m_source,SIGNAL(dataUpdated()),this,SIGNAL(dataUpdated()));
}

void vpDataSet::parserDeleted()
{
    m_source = 0;
    emit parserDied();
}

void vpDataSet::clear()
{
    if(m_source)
    {
        disconnect(m_source,SIGNAL(destroyed()),this,SLOT(parserDeleted()));
        m_source->deleteLater();
        emit parserDied();
        m_source=0;
    }
    if(m_data)
    {
        delete m_data;
        m_data=0;
    }
}

void vpDataSet::importCSV(QString filename)
{
    clear();
    vpParserCSV::csvType ctype = vpParser::guessCSVType(filename);
    if(ctype==vpParserCSV::csvInvalid)
    {
        qDebug()<<"[WARN]vpDataSet::importCSV-> Invalid csv!";
        return;
    }
    else if (ctype==vpParserCSV::csvPlainArray) m_data = new vpDataArrayDoubles;
    else if (ctype==vpParserCSV::csvTimedArray) m_data = new vpDataTimedArray;
    else if (ctype==vpParserCSV::csvGpsArray)   m_data = new vpDataGpsTrack;
    else {qDebug()<<"[ERROR]vpDataSet::importCSV->Invalid csv type:"<<ctype;return;}

    vpParserCSV* parser = new vpParserCSV(m_data);
    setParser(parser);
    parser->openSource(filename);
}

void vpDataSet::importNMEA(QString filename)
{
    clear();
    vpParserNMEA* parser = new vpParserNMEA(new vpDataGpsTrack());
    m_data = parser->data();
    setParser(parser);
    parser->openSource(filename);
}

void vpDataSet::importTorqueLog(QString filename)
{

}

void vpDataSet::importMindflexCSV(QString filename)
{

}

void vpDataSet::connectMindflex(QString macaddr)
{
    clear();
    if(!m_btHelper)
    {
        qDebug()<<"[WARN]vpDataSet::connectMindflex-> Set me a btHelper first!";
        return;
    }
    vpParserMindFlex* parser = new vpParserMindFlex(new vpDataMindFlex(macaddr+"-"+QString::number(QDateTime::currentMSecsSinceEpoch()/1000.0,'f',4))                                                    ,m_btHelper);
    m_data = parser->data();
    setParser(parser);
    parser->openSource(macaddr);
}

void vpDataSet::connectGpsd(QString host, int port)
{
    clear();
    vpParserNMEA* parser = new vpParserNMEA(new vpDataGpsTrack());
    m_data = parser->data();
    setParser(parser);
    parser->openSource("GPSD:"+host+":"+QString::number(port,'f',0));
}
