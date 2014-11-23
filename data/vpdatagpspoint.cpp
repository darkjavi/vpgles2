#include "vpdatagpspoint.h"


vpDataGpsPoint::vpDataGpsPoint(double* time,double* lat,double* lon,double* alt,double* speed,double* bearing,double* acurracy):
    m_time(time),m_latitude(lat),m_longitude(lon),m_altitude(alt),m_speed(speed),
    m_bearing(bearing),m_acurracy(acurracy)
{

}
