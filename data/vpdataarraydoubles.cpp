#include "vpdataarraydoubles.h"

vpDataArrayDoubles::vpDataArrayDoubles(QString id) : vpData(id)
{
    m_haveDataArray=true;
}

void vpDataArrayDoubles::initFields(QVector<QString> fields)
{
    m_fieldNames = fields;
    for( int i = 0 ; i < m_fieldNames.count() ; i++ )
    {
        QVector<double*> column;
        m_columns.append(column);
        m_columnMinVals.append(0);
        m_columnMaxVals.append(0);
        m_columnAverage.append(0);
        m_fieldsColors.append(QColor(Qt::green));
        m_fieldsUnits.append("none");
    }
}

QVector<double> vpDataArrayDoubles::columnToDoubles(int col)
{
    QVector<double> result;
    if(col>m_columns.count())
    {
        qDebug("[WARN]vpDataArrayDoubles::columnToDoubles->index out of range!!");
        return result;
    }
    for(int r = 0 ; r < m_columns[col].count() ; r++)
    {
        result.append(*m_columns[col][r]);
    }
    return result;
}

QVector<QString> vpDataArrayDoubles::columnToStrings(int col)
{
    QVector<QString> result;
    if(col>m_columns.count())
    {
        qDebug("[WARN]vpDataArrayDoubles::columnToString->index out of range!!");
        return result;
    }
    for(int r = 0 ; r < m_columns[col].count() ; r++)
    {
        result.append(QString::number(*m_columns[col][r],'f',4));
    }
    return result;
}

QVector<double> vpDataArrayDoubles::rowToDoubles(int row)
{
    if(row>m_rows.count())
    {
        qDebug("[WARN]vpDataArrayDoubles::rowToDoubles->index out of range!!");
        return QVector<double>();
    }
    return m_rows[row];
}

QVector<QString> vpDataArrayDoubles::rowToStrings(int row)
{
    QVector<QString> result;
    if(row>m_rows.count())
    {
        qDebug("[WARN]vpDataArrayDoubles::rowToStrings->index out of range!!");
        return result;
    }
    for(int i = 0 ; i < m_rows[row].count() ; i++)
    {
        result.append(QString::number(m_rows[row][i],'f',4));
    }
    return result;
}

double vpDataArrayDoubles::posToDouble(quint32 col, quint32 row)
{
    if(row < m_rows.count())
    {
        if(col < m_rows[row].count())
        {
            return m_rows[row][col];
        }
    }

    qDebug()<<"[WARN]vpDataArrayDoubles::posToDouble->out of range! col:"<<col<<"row"<<row ;
    return 1.23456f;
}

QString vpDataArrayDoubles::posToString(quint32 col, quint32 row)
{
    if(row < m_rows.count())
    {
        if(col < m_rows[row].count())
        {
            return QString::number(m_rows[row][col],'f',4);
        }
    }

    qDebug()<<"[WARN]vpDataArrayDoubles::posToDouble->out of range! col:"<<col<<"row"<<row ;
    return QString("1.23456");
}

void vpDataArrayDoubles::append(const QVector<double> &row)
{
    if(m_fieldNames.count() == 0)
    {
        qDebug("[WARN]vpDataArray::append-> Fields are empty! have you called initFields!?");
        return;
    }
    if(row.count()!=m_fieldNames.count())
    {
        qDebug()<<"[WARN]vpDataArray::append-> Unexpected number of fields:"<< row.count() <<"/" <<m_fieldNames.count();
        return;
    }
    m_rows.append(row);
    processRow(m_rows.last());//no se si el last funcionaba para este tipo de uso....
}

double vpDataArrayDoubles::columnMinVal(quint32 col)
{
    if(col > m_columnMinVals.count())
    {
        qDebug()<<"[WARN]vpDataArrayDoubles::columnMinVal-> index out of range!"<< col;
        return 0;
    }
    return m_columnMinVals[col];
}

double vpDataArrayDoubles::columnMaxVal(quint32 col)
{
    if(col > m_columnMaxVals.count())
    {
        qDebug()<<"[WARN]vpDataArrayDoubles::columnMaxVal-> index out of range!"<< col;
        return 0;
    }
    return m_columnMaxVals[col];
}

double vpDataArrayDoubles::columnAvgVal(quint32 col)
{
    if(col > m_columnAverage.count())
    {
        qDebug()<<"[WARN]vpDataArrayDoubles::columnAvgVal-> index out of range!"<< col;
        return 0;
    }
    return m_columnAverage[col];
}

void vpDataArrayDoubles::processRow(QVector<double> &row)
{
    for ( int i = 0 ; i < row.count() ; i++ )
    {
        m_columns[i].append(&row[i]);
        m_columnAverage[i] += (row[i]-m_columnAverage[i])/m_rows.count();
        if(m_rows.count()==0)
        {
            m_columnMinVals[i] = row[i];
            m_columnMaxVals[i] = row[i];
        }
        else if ( row[i] < m_columnMinVals[i] )
        {
            m_columnMinVals[i] = row[i];
        }
        else if ( row[i] > m_columnMaxVals[i] )
        {
            m_columnMaxVals[i] = row[i];
        }
    }
}
