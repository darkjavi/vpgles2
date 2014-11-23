#ifndef VPPARSERSERIALBLUETOOTH_H
#define VPPARSERSERIALBLUETOOTH_H
#include <QProcess>
#include <QTimer>
#include "vpparserserial.h"
#include "bt/bthelper.h"

class vpParserSerialBluetooth : public vpParserSerial
{
    Q_OBJECT
public:
    explicit vpParserSerialBluetooth(QString source, btHelper* bthelper=0, QObject *parent = 0);
    ~vpParserSerialBluetooth();
    bool openSource(QString macaddr);
protected:
    QProcess    m_btPingProcess;
    QTimer      m_btPingTimer;
    btHelper*   m_helper;
    bool        m_sharedBtHelper;
signals:
protected slots:
    void btPing();
    void btPong();
    void btFinished(int);
public slots:
    virtual void closeSource();
};

#endif // VPPARSERSERIALBLUETOOTH_H
