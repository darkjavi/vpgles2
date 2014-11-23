#ifndef VPDATA_H
#define VPDATA_H

#include <QVector>
#include <QString>
#include <QColor>

#include <QDebug>

class vpData
{
public:
    vpData(QString id="none");
    virtual ~vpData(){;}
    const QString&          id()                {return m_id;}
    void                    setID(QString id)   {m_id=id;}
    const QVector<QString>& fields()            {return m_fieldNames;}
    const QVector<QColor>&  fieldsColors()      {return m_fieldsColors;}
    QColor                  fieldColor   (quint32 field);
    void                    setFieldColor(quint32 field, QColor color);
    int                     indexOf      (QString field);
    bool                    haveDataArray()     {return m_haveDataArray;}
    bool                    haveTimeStamp()     {return m_haveTimestamp;}
    bool                    haveGpsTrack()      {return m_haveGpsTrack;}

    virtual void initFields(QVector<QString> fields)=0;
    virtual quint64             count()=0;
    virtual QVector<double>     rowToDoubles(int)           {qDebug("[WARN]vpData::rowToDoubles()->Not implemented in this children!!");    return QVector<double>();}
    virtual QVector<QString>    rowToStrings(int)           {qDebug("[WARN]vpData::rowToStrings()->Not implemented in this children!!");    return QVector<QString>();}
    virtual QVector<double>     columnToDoubles(int)        {qDebug("[WARN]vpData::columnToDoubles()->Not implemented in this children!!"); return QVector<double>();}
    virtual QVector<QString>    columnToStrings(int)        {qDebug("[WARN]vpData::columnToStrings()->Not implemented in this children!!"); return QVector<QString>();}
    virtual QString             posToString(quint32,quint32)    {qDebug("[WARN]vpData::posToString()->Not implemented in this children!!");     return QString("1.23456");}
    virtual double              posToDouble(quint32 ,quint32)   {qDebug("[WARN]vpData::posToDouble()->Not implemented in this children!!");     return 1.23456f;}
    virtual void                append(const QVector<double>&)  {qDebug("[WARN]vpData::append(doubles)->Not implemented in this children!!");}
    virtual void                append(const QVector<QString>&) {qDebug("[WARN]vpData::append(QStrings)->Not implemented in this children!!");}
protected:
    QString             m_id;
    QVector<QString>    m_fieldNames;
    QVector<QString>    m_fieldsUnits;
    QVector<QColor>     m_fieldsColors;
    bool                m_haveDataArray;
    bool                m_haveTimestamp;
    bool                m_haveGpsTrack;
};

#endif // VPDATA_H
