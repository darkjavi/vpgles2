#include "vpdata.h"

vpData::vpData(QObject *parent) :
    QObject(parent)
{
    m_valid = false;
}
vpData::vpData(QVector<qreal> data_row, QObject *parent) :
    QObject(parent)
{
    m_data_row  = data_row;
    m_valid     = true;
}

vpData::vpData(const vpData &obj) :
    QObject(obj.parent())
{
    m_data_row  = obj.m_data_row;
    m_valid     = obj.m_valid;
}

void vpData::operator =(const vpData &obj)
{
    m_data_row  = obj.m_data_row;
    m_valid     = obj.m_valid;
}



void vpData::set_datarow(QVector<qreal> data_row)
{
    m_data_row  = data_row;
    m_valid     = true;
}

QString vpData::info(bool to_screen)
{
    QString info;

    //..................

    if (to_screen)
    {
        qDebug() << info;
    }
    return info;
}
