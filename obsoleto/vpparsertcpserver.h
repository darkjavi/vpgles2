#ifndef VPSOCKETSERVER_H
#define VPSOCKETSERVER_H

#include <QTcpServer>
#include "vpsocketclient.h"
#include "vpparser.h"

class vpParserTCPServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit vpParserTCPServer(QObject *parent = 0);
protected:
    QVector<vpParser*>       m_parsers;
    QVector<vpSocketClient*> m_clients;
    void incomingConnection(int socketDescriptor);
signals:
    void send_addParser(QString name,QVector<QString> fields);
    void send_delParser(QString name);
    void send_appendData(QString name,QVector<double> data);
private slots:
    void addClient(vpSocketClient*);
    void delClient(vpSocketClient*);
    void dataUpdated();
public slots:
    void addParser(vpParser*);
    void delParser(vpParser*);
};

#endif // VPSOCKETSERVER_H
