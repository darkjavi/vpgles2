#include "vpsocketclient.h"
#include <QStringList>

vpSocketClient::vpSocketClient(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(parsePacket()));
    m_pkHeader="[HEADER]";
    m_pkSeparator="[SEP]";
}

void vpSocketClient::parsePacket()
{
    QString str=readAll();
    if(!str.startsWith(m_pkHeader))
    {

    }
    str.remove(0,m_pkHeader.count());
    QStringList d = str.split(m_pkSeparator);
    if(d[0]=="ADDPARSER")
    {
        QString name = d[1];
        QVector<QString> fields;
        for(int f = 2 ; f<d.count() ; f++)
        {
            fields.append(d[f]);
        }
        emit addParser(name,fields);
    } else if(d[0]=="DELPARSER")
    {
        QString name = d[1];
        emit delParser(name);
    } else if(d[0]=="APPENDDATA")
    {
        QString name = d[1];
        QVector<double> data;
        for(int f = 2 ; f<d.count() ; f++)
        {
            data.append(d[f].toDouble());
        }
        emit appendData(name,data);
    }

}

void vpSocketClient::send_addParser(QString name,QVector<QString> fields)
{
    QByteArray d;
    d.append(m_pkHeader).append("ADDPARSER").append(m_pkSeparator);
    d.append(name).append(m_pkSeparator);
    for(int f = 0 ; f < fields.count()-1 ; f++)
    {
        d.append(fields[f]).append(m_pkSeparator);
    }
    d.append(fields.last());
    write(d);
}

void vpSocketClient::send_delParser(QString name)
{
    QByteArray d;
    d.append(m_pkHeader).append("DELPARSER").append(m_pkSeparator);
    d.append(name);
    write(d);
}

void vpSocketClient::send_appendData(QString name, QVector<double> data)
{
    QByteArray d;
    d.append(m_pkHeader).append("APPENDDATA").append(m_pkSeparator);
    d.append(name).append(m_pkSeparator);
    for(int f = 0 ; f < data.count()-1 ; f++)
    {
        d.append(QString::number(data[f],'f',4)).append(m_pkSeparator);
    }
    d.append(QString::number(data.last(),'f',4));
    write(d);
}
