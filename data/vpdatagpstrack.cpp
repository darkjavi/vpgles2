#include "vpdatagpstrack.h"

vpDataGpsTrack::vpDataGpsTrack(QString id) : vpDataTimedArray(id),m_latFieldFound(false),m_lonFieldFound(false),m_altFieldFound(false),m_spdFieldFound(false),m_beaFieldFound(true),m_acuFieldFound(false)
{
    m_haveDataArray=false;
    m_haveTimestamp=true;
    m_haveGpsTrack=true;
}

void vpDataGpsTrack::initFields(QVector<QString> fields)
{
    vpDataTimedArray::initFields(fields);
    for(int i = 0 ; i < fields.count() ; i++)
    {
        if( (!m_latFieldFound) && (fields[i].contains("latitude",Qt::CaseInsensitive))){

            qDebug() << "[DEBUG]vpDataTimedArray::initFields-> latitude field:" << fields[i] << " index" << i;
            setLatitudeField(i);
        }
        else if( (!m_lonFieldFound) && (fields[i].contains("longitude",Qt::CaseInsensitive))){

            qDebug() << "[DEBUG]vpDataTimedArray::initFields-> longitude field:" << fields[i] << " index" << i;
            setLongitudeField(i);
        }
        else if( (!m_altFieldFound) && (fields[i].contains("altitude",Qt::CaseInsensitive))){

            qDebug() << "[DEBUG]vpDataTimedArray::initFields-> altitude field:" << fields[i] << " index" << i;
            setLatitudeField(i);
        }
        else if( (!m_spdFieldFound) && (fields[i].contains("speed",Qt::CaseInsensitive))){

            qDebug() << "[DEBUG]vpDataTimedArray::initFields-> speed field:" << fields[i] << " index" << i;
            setLatitudeField(i);
        }
        else if( (!m_beaFieldFound) && (fields[i].contains("bearing",Qt::CaseInsensitive))){

            qDebug() << "[DEBUG]vpDataTimedArray::initFields-> bearing field:" << fields[i] << " index" << i;
            setBearingField(i);
        }
        else if( (!m_acuFieldFound) && (fields[i].contains("satellite",Qt::CaseInsensitive))){

            qDebug() << "[DEBUG]vpDataTimedArray::initFields-> acurracy field:" << fields[i] << " index" << i;
            setAcurracyField(i);
        }
    }
    if(!m_latFieldFound)qDebug("[WARN]vpDataGpsTrack::initFields-> Cannot guess latitude field, use setLatitudeField()");
    if(!m_lonFieldFound)qDebug("[WARN]vpDataGpsTrack::initFields-> Cannot guess longitude field, use setLongitudeField()");
    if(!m_altFieldFound)qDebug("[WARN]vpDataGpsTrack::initFields-> Cannot guess altitude field, use setaltitudeField()");
    if(!m_spdFieldFound)qDebug("[WARN]vpDataGpsTrack::initFields-> Cannot guess speed field, use setSpèedField()");
    if(!m_beaFieldFound)qDebug("[WARN]vpDataGpsTrack::initFields-> Cannot guess bearing field, use setBearingField()");
    if(!m_acuFieldFound)qDebug("[WARN]vpDataGpsTrack::initFields-> Cannot guess acurracy field, use setAcurracyField()");
}

void vpDataGpsTrack::append(QVector<double> &row)
{
    vpDataTimedArray::append(row);
    if(gpsFieldsFound())
    {
        vpDataGpsPoint p = generateGpsPoint(row);
        appendGpsPoint(p);
    }
}

void vpDataGpsTrack::setLatitudeField(int index)
{
    if(index > m_fieldNames.count())
    {
        qDebug()<<"[WARN]vpDataTimedArray::setLatitudeField->index out of range!"<<index<<"/"<<m_fieldNames.count();
    }
    m_latField=index;
    m_latFieldFound=true;
}

void vpDataGpsTrack::setLongitudeField(int index)
{
    if(index > m_fieldNames.count())
    {
        qDebug()<<"[WARN]vpDataTimedArray::setLongitudeField->index out of range!"<<index<<"/"<<m_fieldNames.count();
    }
    m_lonField=index;
    m_lonFieldFound=true;
}
void vpDataGpsTrack::setAltitudeField(int index)
{
    if(index > m_fieldNames.count())
    {
        qDebug()<<"[WARN]vpDataTimedArray::setAltitudeField->index out of range!"<<index<<"/"<<m_fieldNames.count();
    }
    m_altField=index;
    m_altFieldFound=true;
}
void vpDataGpsTrack::setSpeedField(int index)
{
    if(index > m_fieldNames.count())
    {
        qDebug()<<"[WARN]vpDataTimedArray::setSpeedField->index out of range!"<<index<<"/"<<m_fieldNames.count();
    }
    m_spdField=index;
    m_spdFieldFound=true;
}
void vpDataGpsTrack::setBearingField(int index)
{
    if(index > m_fieldNames.count())
    {
        qDebug()<<"[WARN]vpDataTimedArray::setBearingField->index out of range!"<<index<<"/"<<m_fieldNames.count();
    }
    m_beaField=index;
    m_beaFieldFound=true;
}
void vpDataGpsTrack::setAcurracyField(int index)
{
    if(index > m_fieldNames.count())
    {
        qDebug()<<"[WARN]vpDataTimedArray::setAcurracyField->index out of range!"<<index<<"/"<<m_fieldNames.count();
    }
    m_acuField=index;
    m_acuFieldFound=true;
}

bool vpDataGpsTrack::gpsFieldsFound()
{
    return  m_timeFieldFound&&m_latFieldFound&&m_lonFieldFound&&m_altFieldFound&&
            m_spdFieldFound&&m_beaFieldFound&&m_acuFieldFound;
}

vpDataGpsPoint vpDataGpsTrack::generateGpsPoint(QVector<double> &row)
{
    return vpDataGpsPoint(&row[m_timeField],&row[m_latField],&row[m_lonField],&row[m_altField],
                          &row[m_spdField],&row[m_beaField],&row[m_acuField]);
}

void vpDataGpsTrack::appendGpsPoint(vpDataGpsPoint p)
{
    if(m_gpsSegments.isEmpty())
        m_gpsSegments.append(vpDataGpsSegment(m_id+"-"+QString::number(m_gpsSegments.count())));
    m_gpsPoints.append(p);
    m_gpsSegments.last().appendGpsPoint(&m_gpsPoints.last());//no estoy seguro del last
}

void vpDataGpsTrack::rebuildGpsTrack()
{
    m_gpsSegments.clear();
    m_gpsPoints.clear();
    if(!gpsFieldsFound())
    {
        qDebug() << "[WARN]vpDataGpsTrack::rebuildGpsTrack()-> can not rebuild gps track: not all fields are set!";
        return;
    }
    for( int i = 0 ; i < m_rows.count() ; i++ )
    {
        vpDataGpsPoint p = generateGpsPoint(m_rows[i]);
        appendGpsPoint(p);
    }
}
