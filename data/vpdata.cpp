#include "vpdata.h"

vpData::vpData(QString id) :m_id(id), m_haveDataArray(false),m_haveTimestamp(false),m_haveGpsTrack(false)
{
}

QColor vpData::fieldColor(quint32 field)
{
    if(field>m_fieldsColors.count())
    {
        qDebug()<<"[WARN]vpData::fieldColor->Index out of range!"<<field;
        return QColor();
    }
    return m_fieldsColors[field];
}

void vpData::setFieldColor(quint32 field,QColor color)
{
    if(field>m_fieldsColors.count())
    {
        qDebug()<<"[WARN]vpData::setFieldColor->Index out of range!"<<field;
        return;
    }
    m_fieldsColors[field]=color;
}

int vpData::indexOf(QString field)
{
    for(int i = 0 ; i < m_fieldNames.count() ; i++)
    {
        if(m_fieldNames[i]==field)
        {
            return i;
        }
    }
    qDebug()<<"[WARN]vpData::indexOf->Can't find field:"<<field<<"m_fields:"<<m_fieldNames;
    return -1;
}
