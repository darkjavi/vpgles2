#include "vpparsertcpclient.h"

vpParserTCPClient::vpParserTCPClient(QObject *parent) :
    QObject(parent)
{
    connect(&m_socket,SIGNAL(addParser(QString,QVector<QString>)),this,SLOT(addParser(QString,QVector<QString>)));
    connect(&m_socket,SIGNAL(delParser(QString)),this,SLOT(delParser(QString)));
    connect(&m_socket,SIGNAL(appendData(QString,QVector<double>)),this,SLOT(appendData(QString,QVector<double>)));
}

bool vpParserTCPClient::openSocket(QString host, quint16 port)
{
    m_socket.connectToHost(host,port);
}

void vpParserTCPClient::addParser(QString name, QVector<QString> fields)
{
    //vpParserRemoteParser* p = new vpParserRemoteParser( this);
    //m_parsers.append(p);
    //connect(p,SIGNAL(data_updated()),p,SIGNAL(data_rx()));
    //emit newParser(p);
}

void vpParserTCPClient::delParser(QString name)
{

}

void vpParserTCPClient::appendData(QString name, QVector<double> data)
{
    for(int p = 0 ; p<m_parsers.count() ;p++)
    {
        if(m_parsers[p]->source()==name)
        {
            //m_parsers[p]->dataset().append(data);
            return;
        }
    }
}

