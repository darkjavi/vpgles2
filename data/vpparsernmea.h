#ifndef VPPARSERNMEA_H
#define VPPARSERNMEA_H

#include "vpparser.h"
//#include "vpdatagpstrack.h"
#include <QTcpSocket>
#include <QFile>
#include <QProcess>
class vpDataSet;
class vpParserNMEA : public vpParser
{
    Q_OBJECT
public:
    explicit vpParserNMEA(QString source ="none", QObject *parent = 0);
    bool openSource(QString source);
    bool        isOpen();
    bool        startGpsd();
    void        stopGpsd();
    static bool         isGpsdWorking(QString gpsdHost, int gpsdPort);
    static QStringList  getGpsCandidates();
protected:
    QString     m_gpsdHost;
    int         m_gpsdPort;
    QProcess    m_gpsdProcess;
    QTcpSocket  m_socket;
    QString     m_buffer;
    void        parseBuffer();
    bool        connectGpsd(QString host,int port);
signals:

protected slots:
    void    readGpsdSocket();

public slots:
    void    closeSource();
};

#endif // VPPARSERNMEA_H
