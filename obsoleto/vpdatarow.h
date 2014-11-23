#ifndef VPDATAROW_H
#define VPDATAROW_H

#include <QVector>
#include "vpdatagpspoint.h"

class vpDataRow
{

public:
    enum dataType
    {
        dataInvalid,
        dataDoubleArray,
        dataTimedDoubleArray,
        dataGpsTrack,
        dataGpsArray
    };
    vpDataRow();
    vpDataRow(QVector<qreal> data);
    vpDataRow(QDateTime time, QVector<qreal> data);
    vpDataRow(vpDataGpsPoint gps_data);
    vpDataRow(vpDataGpsPoint gps_data,QVector<qreal> data);
    ~vpDataRow();

    void set_data(QVector<qreal> data);
    void set_data(QDateTime time, QVector<qreal> data);
    void set_data(vpDataGpsPoint gps_data);
    void set_data(vpDataGpsPoint gps_data,QVector<qreal> data);

    QVector<qreal>   get_raw_row();
    QDateTime        get_time();
    QVector<qreal>&  get_data_row();
    vpDataGpsPoint*  get_gps_data();

    quint8 get_type()   {return m_dataType;}
protected:
    quint8  m_dataType;

    QVector<qreal>  m_doubles;
    vpDataGpsPoint  *m_gpspoint;
    QDateTime       *m_time;
};

#endif // VPDATAROW_H
