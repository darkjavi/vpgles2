#ifndef VPDATACOLUMN_H
#define VPDATACOLUMN_H

#include <QObject>
#include "vpdata.h"

class vpDataColumn : public QObject
{
    Q_OBJECT
public:
    explicit vpDataColumn(QObject *parent = 0);
    vpDataColumn(QVector<vpData> data,QString fieldname = "NONE", QObject *parent = 0);
    vpDataColumn(const vpDataColumn &obj);
    void operator =(const vpDataColumn& obj);

    QString& fieldname()        {return m_fieldname;}
    QVector<vpData>& data()     {return m_data;}
    void append(vpData& data)   {m_data.append(data);}
    bool is_valid(){return m_valid;}

private:
    QString          m_fieldname;
    QVector<vpData>  m_data;
    bool             m_valid;
signals:
    
public slots:
    
};

#endif // VPDATACOLUMN_H
