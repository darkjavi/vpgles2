#include "vpparserserialbluetooth.h"

vpParserSerialBluetooth::vpParserSerialBluetooth(QString source, btHelper *bthelper, QObject *parent) :
    vpParserSerial(source,parent)
{
    if(bthelper)
    {
        m_helper=bthelper;
        m_sharedBtHelper = true;
    }
    else
    {
        m_helper = new btHelper();
        m_sharedBtHelper = false;
    }
    m_btPingTimer.setInterval(10000);
    connect(&m_btPingTimer,SIGNAL(timeout()),this,SLOT(btPing()));
    connect(&m_btPingProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(btPong()));
    connect(&m_btPingProcess,SIGNAL(finished(int)),this,SLOT(btFinished(int)));
}

vpParserSerialBluetooth::~vpParserSerialBluetooth()
{
    if(m_source.isEmpty())
        return;
    if(m_helper->macIsBind(m_source))
    {
        m_helper->release(m_source);
    }
    if(!m_sharedBtHelper)
    {
        m_helper->deleteLater();
    }
}

bool vpParserSerialBluetooth::openSource(QString macaddr)
{
    if(macaddr.isEmpty())
    {
        qDebug()<<"[WARN]vpParserSerialBluetooth::openSource-> Empty macaddr!";
        return false;
    }
    if(!m_helper->macIsBind(macaddr))
    {
        m_helper->bind(macaddr);
    }
    QString commdevice=m_helper->get_deviceBind(macaddr);
    if(!commdevice.isEmpty())
    {
        if(!vpParserSerial::openSource(commdevice)) return false;
        m_source = macaddr;
        m_btPingTimer.start();
        return true;
    }
    return false;
}
//protected slots
void vpParserSerialBluetooth::btPing()
{
    //m_btPingProcess.start("sudo l2ping -c1 "+m_source);
}

void vpParserSerialBluetooth::btPong()
{
    QString raw=m_btPingProcess.readAll();
    QStringList output= raw.split("\n");
    if( (output.count()>=3) && (output.at(1).contains("time")) )
    {
        qDebug()<<output.at(1).split("time").at(1);
    }
}

void vpParserSerialBluetooth::btFinished(int status)
{
    if(status==0) return;
    qDebug()<<"[WARN]vpParserSerialBluetooth::btFinished->Ping process ended with status:"<<status;
    qDebug()<<"[WARN]vpParserSerialBluetooth::btFinished->Stopping capture!"<<status;

    QObject::deleteLater(); //esto hara el cast apropiado????
}

//public slots

void vpParserSerialBluetooth::closeSource()
{
    if(m_helper->macIsBind(m_source))
    {
        m_helper->release(m_source);
    }
    vpParserSerial::closeSource();
}
