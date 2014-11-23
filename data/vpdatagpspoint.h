#ifndef VPDATAGPSPOINT_H
#define VPDATAGPSPOINT_H

#include <QDebug>

class vpDataGpsPoint
{
public:
    vpDataGpsPoint(){m_acurracy = 0;}
    vpDataGpsPoint(double* time,double* lat,double* lon,double* alt,double* speed,double* bearing,double* acurracy);

    double timeStamp()  {return *m_time;}
    double lat()        {return *m_latitude;}
    double lon()        {return *m_longitude;}
    double alt()        {return *m_altitude;}
    double speed()      {return *m_speed;}
    double bearing()    {return *m_bearing;}
    double acurracy()   {return *m_acurracy;}

    bool   isValid()    {return (m_time) && (*m_time>0) && (*m_acurracy>0);}
protected:
    double*  m_time;
    double*  m_latitude;
    double*  m_longitude;
    double*  m_altitude;
    double*  m_speed;
    double*  m_bearing;
    double*  m_acurracy;
};

#endif // VPDATAGPSPOINT_H
