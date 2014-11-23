#ifndef VPLIVESOURCE_H
#define VPLIVESOURCE_H

#include <QThread>
#include <qextserialport.h>


class vpLiveSource : public QThread
{
    Q_OBJECT
public:
    explicit vpLiveSource(QObject *parent = 0);
    bool set_serial_source(QString device);
private:
    QextSerialPort m_comm_port;
    void run();
signals:
    void data_avaiable();
public slots:
    
};

#endif // VPLIVESOURCE_H
