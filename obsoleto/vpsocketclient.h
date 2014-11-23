#ifndef VPSOCKETCLIENT_H
#define VPSOCKETCLIENT_H

#include <QTcpSocket>

class vpSocketClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit vpSocketClient(QObject *parent = 0);
private:
    QString m_pkHeader;
    QString m_pkSeparator;
signals:
    void addParser(QString name,QVector<QString> fields);
    void delParser(QString name);
    void appendData(QString name,QVector<double> data);

private slots:
    void parsePacket();
public slots:
    void send_addParser(QString name,QVector<QString> fields);
    void send_delParser(QString name);
    void send_appendData(QString name,QVector<double> data);
};

#endif // VPSOCKETCLIENT_H
