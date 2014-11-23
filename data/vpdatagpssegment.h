#ifndef VPDATAGPSSEGMENT_H
#define VPDATAGPSSEGMENT_H

#include <QVector>
#include "vpdatagpspoint.h"

class vpDataGpsSegment
{
public:
    vpDataGpsSegment(QString id="none");
    void appendGpsPoint(vpDataGpsPoint* p);

    QString id()    {return m_id;}
protected:
    QString m_id;
    QVector<vpDataGpsPoint*>    m_gpsPoints;
};

#endif // VPDATAGPSSEGMENT_H
