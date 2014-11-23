#ifndef VPDATAGPSTRACK_H
#define VPDATAGPSTRACK_H

#include "vpdatatimedarray.h"
#include "vpdatagpssegment.h"

class vpDataGpsTrack : public vpDataTimedArray
{
public:
    vpDataGpsTrack(QString id = "none");
    virtual void initFields(QVector<QString> fields);
    virtual void append(QVector<double> &row);
    void setLatitudeField(int index);
    void setLongitudeField(int index);
    void setAltitudeField(int index);
    void setSpeedField(int index);
    void setBearingField(int index);
    void setAcurracyField(int index);
    bool gpsFieldsFound();
    void rebuildGpsTrack();
protected:
    vpDataGpsPoint generateGpsPoint(QVector<double> &row);
    void appendGpsPoint(vpDataGpsPoint p);

    QVector<vpDataGpsSegment>   m_gpsSegments;
    QVector<vpDataGpsPoint>     m_gpsPoints;

    quint32                     m_latField;
    bool                        m_latFieldFound;
    quint32                     m_lonField;
    bool                        m_lonFieldFound;
    quint32                     m_altField;
    bool                        m_altFieldFound;
    quint32                     m_spdField;
    bool                        m_spdFieldFound;
    quint32                     m_beaField;
    bool                        m_beaFieldFound;
    quint32                     m_acuField;
    bool                        m_acuFieldFound;
};

#endif // VPDATAGPSTRACK_H
