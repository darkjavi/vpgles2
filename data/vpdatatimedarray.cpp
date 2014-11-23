#include "vpdatatimedarray.h"

vpDataTimedArray::vpDataTimedArray(QString id) : vpDataArrayDoubles(id), m_timeFieldFound(false)
{
    m_haveDataArray=true;
    m_haveTimestamp=true;
    m_haveGpsTrack=false;
}

void vpDataTimedArray::initFields(QVector<QString> fields)
{
    vpDataArrayDoubles::initFields(fields);
    for(int i = 0 ; i < fields.count() ; i++)
    {
        if( (fields[i].contains("timestamp",Qt::CaseInsensitive)) ||
            (fields[i].contains("time"),Qt::CaseInsensitive) )
        {

            qDebug() << "[DEBUG]vpDataTimedArray::initFields-> Time field:" << fields[i] << " index" << i;
            setTimeField(i);
            break;
        }
    }
    if(!m_timeFieldFound)
    {
        qDebug() << "[WARN!]vpDataTimedArray::initFields-> Could not guess time field! Use setTimeField";
    }

}

void vpDataTimedArray::append(const QVector<double> &row)
{
    vpDataArrayDoubles::append(row);
}

void vpDataTimedArray::setTimeField(quint32 index)
{
    if(index > m_fieldNames.count())
    {
        qDebug()<<"[WARN]vpDataTimedArray::setTimeField->index out of range!"<<index<<"/"<<m_fieldNames.count();
    }
    m_timeField=index;
    m_timeFieldFound=true;
}
