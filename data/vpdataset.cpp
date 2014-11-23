#include "vpdataset.h"
#include <QDebug>

//CONSTRUCTORES
vpDataSet::vpDataSet(QObject *parent) :
    QObject(parent) , m_auxDataSet(0),m_id("dummy"),m_storeInStrings(false),
    m_fullLoad(true),m_cacheStart(0),m_cacheCount(500),m_dataSource(0)
{
    m_dataSource = new vpDataSource(this);
}
vpDataSet::~vpDataSet()
{
    if(m_auxDataSet)
        m_auxDataSet->deleteLater();
    if(m_dataSource)
        m_dataSource->deleteLater();
}
/////
/// \brief vpDataSet::vpDataSet
/// \param obj
vpDataSet::vpDataSet(const vpDataSet &obj) :
    QObject()
{
    *this=obj;
}
void vpDataSet::operator =(const vpDataSet &obj)
{

}
//METODOS

///
/// \brief vpDataSet::open
/// \param resource
/// \param full_load
/// \return
///
bool    vpDataSet::open(QString resource, bool full_load)
{}
void    vpDataSet::initDataset(QString id,QVector<QString> fields,bool willStoreStrings)
{
    m_fields = fields;
    m_storeInStrings = willStoreStrings;
    setId(id);
}
int vpDataSet::indexOf(QString field)
{
    return m_fields.indexOf(field);
}
QVector<double>  vpDataSet::getRowDoubles(uint row)
{
    if(row >= getRowCount())
    {
        return QVector<double>();
    }
    if(m_storeInStrings)
    {
        QVector<double> row_d;
        for(int i = 0 ; i < m_dataStrs[row].count() ; i++)
        {
            row_d.append(m_dataStrs[row][i].toDouble());
        }
        return row_d;
    }
    else
    {
        return m_dataDoubles[row];
    }
}
QVector<double>  vpDataSet::getColumnDoubles(uint col)
{

}
double  vpDataSet::getPosDouble(uint row, uint col)
{
    if(row >= getRowCount())
    {
        return 0;
    }
    if(m_storeInStrings)
    {
        if(col >= m_dataStrs[row].count()) return 0;
        return m_dataStrs[row][col].toDouble();
    }
    else
    {
        if(col >= m_dataDoubles[row].count()) return 0;
        return m_dataDoubles[row][col];
    }
}
QVector<QString> vpDataSet::getRowStrings(uint row)
{
    if(row >= getRowCount())
    {
        return QVector<QString>();
    }
    if(m_storeInStrings)
    {
        return m_dataStrs[row];
    }
    else
    {
        QVector<QString> row_s;
        for(int i = 0 ; i < m_dataDoubles[row].count() ; i++)
        {
            row_s.append(QString::number(m_dataDoubles[row][i],'f',4));
        }
        return row_s;
    }
}
QVector<QString> vpDataSet::getColumnStrings(uint col)
{

}
QString     vpDataSet::getPosString(uint row, uint col)
{
    if(row >= getRowCount())
    {
        return "Err";
    }
    if(m_storeInStrings)
    {
        if(col >= m_dataStrs[row].count()) return "Err";
        return m_dataStrs[row][col];
    }
    else
    {
        if(col >= m_dataDoubles[row].count()) return "Err";
        return QString::number(m_dataDoubles[row][col],'f',4);
    }
}
vpDataSet   vpDataSet::getSubDataSet(QVector<QString> fields,uint start, uint count)
{

}
///////////////////////
///SLOTS

void vpDataSet::appendRowDoubles(const QVector<double> row, QVector<QString> fields)
{
    if(fields.count()!=0)
    {
        qDebug()<<"[WARN]appendRowDoubles-> Selective append not yet implemented!";
        //return;
    }
    if(row.count()!=m_fields.count())
    {
        qDebug()<<"[WARN]appendRowDoubles-> fields do not match!" << row.count() <<"/"<< m_fields.count();
        return;
    }
    if(m_storeInStrings)
    {
        QVector<QString> row_s;
        for(int c = 0 ; c < row.count() ; c++)
        {
            row_s.append(QString::number(row[c],'f',4));
        }
        m_dataStrs.append(row_s);
    }
    else
    {
        m_dataDoubles << row;
    }
}

void vpDataSet::appendRowStrings(const QVector<QString> row, QVector<QString> fields)
{
    if(fields.count()!=0)
    {
        qDebug()<<"[WARN]appendRowStrings-> Selective append not yet implemented!";
        //return;
    }
    if(row.count()!=m_fields.count())
    {
        qDebug()<<"[WARN]appendRowStrings-> fields do not match!" << row.count() <<"/"<< m_fields.count();
        return;
    }
    if(m_storeInStrings)
    {
        m_dataStrs << row;
    }
    else
    {
        QVector<double> row_d;
        for(int c = 0 ; c < row.count() ; c++)
        {
            row_d.append(row[c].toDouble());
        }
        m_dataDoubles.append(row_d);
    }
}

void vpDataSet::appendDataDoubles(const QVector<QVector<double> >  data, QVector<QString> fields)
{
    if(!data.count())
    {
        qDebug()<<"[WARN]appendDataDoubles-> dataset is empty!!";
    }
    if(fields.count()!=0)
    {
        qDebug()<<"[WARN]appendDataDoubles-> Selective append not yet implemented!";
        return;
    }
    if(data[0].count()!=m_fields.count())
    {
        qDebug()<<"[WARN]appendDataDoubles-> fields do not match!" << data.count() <<"/"<< m_fields.count();
        return;
    }
    if(m_storeInStrings)
    {
        for(int r = 0 ; r < data.count() ; r++)
        {
            QVector<QString> row;
            for( int c = 0; c < data[r].count() ; c++)
                row.append(QString::number(data[r][c],'f',4));
            m_dataStrs.append(row);
        }
    }
    else
    {
        m_dataDoubles << data;
    }
}

void    vpDataSet::appendDataStrings(const QVector<QVector<QString> > data, QVector<QString> fields)
{
    if(!data.count())
    {
        qDebug()<<"[WARN]appendDataStrings-> dataset is empty!!";
    }
    if(fields.count()!=0)
    {
        qDebug()<<"[WARN]appendDataStrings-> Selective append not yet implemented!";
        //return;
    }
    if(data[0].count()!=m_fields.count())
    {
        qDebug()<<"[WARN]appendDataStrings-> fields do not match!" << data.count() <<"/"<< m_fields.count();
        return;
    }
    if(m_storeInStrings)
    {
        m_dataStrs << data;
    }
    else
    {
        for(int r = 0 ; r < data.count() ; r++)
        {
            QVector<double> row;
            for( int c = 0; c < data[r].count() ; c++)
                row.append(data[r][c].toDouble());
            m_dataDoubles.append(row);
        }
    }
}


void vpDataSet::load()
{}
void vpDataSet::unload()
{}
void vpDataSet::clear()
{}
