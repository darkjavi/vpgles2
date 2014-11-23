#include "vpparsertcpserver.h"

vpParserTCPServer::vpParserTCPServer(QObject *parent) :
    QTcpServer(parent)
{
}

void vpParserTCPServer::incomingConnection(int socketDescriptor)
{
    vpSocketClient* client = new vpSocketClient(this);
    if (client->setSocketDescriptor(socketDescriptor))
    {
        addClient(client);
    }
    else
    {
        client->deleteLater();
    }
}

void vpParserTCPServer::addClient(vpSocketClient* c)
{
    connect(c,SIGNAL(destroyed(QObject*)),this,SLOT(delClient(vpSocketClient*)));
    m_clients.append(c);
    for(int p=0;p<m_parsers.count();p++)
    {
        //c->send_addParser(m_parsers[p]->source(),m_data.ields());
    }
    connect(this,SIGNAL(send_addParser(QString,QVector<QString>)),c,SLOT(send_addParser(QString,QVector<QString>)));
    connect(this,SIGNAL(send_delParser(QString)),c,SLOT(send_delParser(QString)));
    connect(this,SIGNAL(send_appendData(QString,QVector<double>)),c,SLOT(send_appendData(QString,QVector<double>)));
}

void vpParserTCPServer::delClient(vpSocketClient* c)
{

}

void vpParserTCPServer::addParser(vpParser* p)
{
    connect(p,SIGNAL(destroyed(QObject*)),this,SLOT(delParser(vpParser*)));
    m_parsers.append(p);
    connect(p,SIGNAL(destroyed(QObject*)),this,SLOT(delParser(vpParser*)));
    connect(p,SIGNAL(data_updated()),this,SLOT(dataUpdated()));
    //emit send_addParser(p->source(),p->dataset().getFields());
}

void vpParserTCPServer::delParser(vpParser * p)
{

}

void vpParserTCPServer::dataUpdated()
{
    vpParser* p = static_cast<vpParser*>(sender());
    if(!p) return;
    //emit send_appendData(p->source(),p->dataset().lastRowDoubles());
}
