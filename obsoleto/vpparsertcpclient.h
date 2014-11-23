#ifndef VPPARSERTCPCLIENT_H
#define VPPARSERTCPCLIENT_H

#include <QObject>
#include "vpparserremoteparser.h"
#include "vpsocketclient.h"

class vpParserTCPClient : public QObject
{
    Q_OBJECT
public:
    explicit vpParserTCPClient(QObject *parent = 0);
    bool     openSocket(QString host,quint16 port);
private:
    QVector<vpParserRemoteParser*> m_parsers;
    vpSocketClient                 m_socket;
signals:
    void    newParser(vpParser*);
signals:

public slots:
    void addParser(QString name,QVector<QString> fields);
    void delParser(QString name);
    void appendData(QString name,QVector<double> data);
};
#endif // VPPARSERTCPCLIENT_H
