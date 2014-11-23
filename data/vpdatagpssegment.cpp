#include "vpdatagpssegment.h"

vpDataGpsSegment::vpDataGpsSegment(QString id) : m_id(id)
{
}

void vpDataGpsSegment::appendGpsPoint(vpDataGpsPoint *p)
{
    m_gpsPoints.append(p);
}
