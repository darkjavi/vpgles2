#include "vpdataset.h"

vpDataSet::vpDataSet(QVector<QString> fields, QString id,quint8 dataType) :
    QObject()
{
    m_id = id;
    m_data_type = dataType;
    init_data_fields(fields);
}

vpDataSet::vpDataSet(const vpDataSet &obj) : QObject()
{
    //REVISAR SI ESTA TODO!
    m_id            = obj.m_id+"-c";
    m_data_type     = obj.m_data_type;
    m_rowsDoubles   = obj.m_rowsDoubles;
    m_rowsStrings   = obj.m_rowsStrings;
    m_data_fields   = obj.m_data_fields;
    m_fields_unit   = obj.m_fields_unit;
    m_fieldsColors  = obj.m_fieldsColors;

    m_colum_minvals = obj.m_colum_minvals;
    m_colum_maxvals = obj.m_colum_maxvals;
    m_colum_average = obj.m_colum_average;

    m_columns.clear();
    for( int i = 0 ; i < m_data_fields.count() ; i++ )
    {
        QVector<double*> column;
        m_columns.append(column);
        for( int r = 0; r< m_rowsDoubles.count() ; r++ )
        {
            m_columns[i].append(&m_rowsDoubles[r][i]);
        }
    }
}

void vpDataSet::operator =(const vpDataSet& obj)
{
    //REVISAR SI ESTA TODO!
    m_id            = obj.m_id+"-op=";
    m_data_type     = obj.m_data_type;
    m_rowsDoubles   = obj.m_rowsDoubles;
    m_rowsStrings   = obj.m_rowsStrings;
    m_data_fields   = obj.m_data_fields;
    m_fields_unit   = obj.m_fields_unit;
    m_fieldsColors  = obj.m_fieldsColors;

    m_colum_minvals = obj.m_colum_minvals;
    m_colum_maxvals = obj.m_colum_maxvals;
    m_colum_average = obj.m_colum_average;

    m_columns.clear();
    for( int i = 0 ; i < m_data_fields.count() ; i++ )
    {
        QVector<double*> column;
        m_columns.append(column);
        for( int r = 0; r< m_rowsDoubles.count() ; r++ )
        {
            m_columns[i].append(&m_rowsDoubles[r][i]);
        }
    }
}

void vpDataSet::init_data_fields(QVector<QString> data_fields)
{
    //if(m_data_type==vpDataSet::datasetDoubleArray)
    m_data_fields = data_fields;
    for( int i = 0 ; i < m_data_fields.count() ; i++ )
    {
        QVector<double*> column;
        m_columns.append(column);
        m_colum_minvals.append(0);
        m_colum_maxvals.append(0);
        m_colum_average.append(0);
        m_fieldsColors.append(Qt::white);
    }
    emit data_updated();
}

void vpDataSet::set_fieldColor(int index, QColor n_color)
{
    if (index > m_fieldsColors.count())
    {
        qDebug() << "[WARN]vpDataSet::set_fieldColor-> index solicitado fuera de rango:" << index << "/" << m_fieldsColors.count();
        return;
    }
    m_fieldsColors[index] = n_color;
    emit data_updated();
}

QColor vpDataSet::get_fieldColor(int index)
{
    if (index > m_fieldsColors.count())
    {
        qDebug() << "[WARN]vpDataSet::get_fieldColor-> index solicitado fuera de rango:" << index << "/" << m_fieldsColors.count();
        return(Qt::white);
    }
    return m_fieldsColors[index];
}

void vpDataSet::append(QVector<double>   row_doubles,QVector<QString> row_strs)
{
    int total_fields = row_doubles.count()+row_strs.count();
    if( total_fields != m_data_fields.count())
    {
        qDebug() << "[ERROR]vpDataSet::append->El tamaño de los datos no coincide campos/esperados:" << total_fields << "/" << m_data_fields.count();
        return;
    }
    m_rowsDoubles.append(row_doubles);
    m_rowsStrings.append(row_strs);
    append_to_stats(row_doubles);
    emit data_updated();
}

void vpDataSet::append(QVector<double>   row_doubles)
{
    if( row_doubles.count() != m_data_fields.count())
    {
        qDebug() << "[ERROR]vpDataSet::append->El tamaño de los datos no coincide campos/esperados:" << row_doubles.count() << "/" << m_data_fields.count();
        return;
    }
    m_rowsDoubles.append(row_doubles);
    append_to_stats(row_doubles);
    emit new_data(m_rowsDoubles.last());
    emit data_updated();
}

void vpDataSet::append(QVector<QString> row_strs)
{
    if( row_strs.count() != m_data_fields.count())
    {
        qDebug() << "[ERROR]vpDataSet::append->El tamaño de los datos no coincide campos/esperados:" << row_strs.count() << "/" << m_data_fields.count();
        return;
    }
    m_rowsStrings.append(row_strs);
    emit data_updated();
}

void vpDataSet::append_to_stats(QVector<double>& last)
{
    for ( int i = 0 ; i < last.count() ; i++ )
    {
        m_columns[i].append(&last[i]);

        m_colum_average[i] += (last[i]-m_colum_average[i])/count_rows() ;

        if ( last[i] < m_colum_minvals[i] )
        {
            m_colum_minvals[i] = last[i];
        }
        else if ( last[i] > m_colum_maxvals[i] )
        {
            m_colum_maxvals[i] = last[i];
        }
    }
}

void vpDataSet::reset_stats()
{
    //IMPLEMENTAME!
    //vovler a calcular todas las medias/stats.....
}

QVector<double*>& vpDataSet::column(QString fieldname)
{
    qint16 field_index = indexOf(fieldname);
    if ( field_index == -1 )
    {
        qDebug() << "[ERROR]vpDataSet::data_column-> no encuentro el campo:" << fieldname << "devuelvo el primero..." ;
        return column(0);
    }
    return column(field_index);
}

QVector<double*>& vpDataSet::column(ushort index)
{
    if (index >= m_columns.count())
    {
        qDebug() << "[ERROR]vpDataSet::data_column-> indice fuera de rango:" << index << "se devuelve la primera columna";
        return m_columns[0];
    }
    else
    {
        return m_columns[index];
    }
}

double vpDataSet::column_min_value(ushort index)
{
    if (index >= m_colum_minvals.count())
    {
        qDebug() << "[ERROR]vpDataSet::data_min_value-> indice fuera de rango:" << index;
        return m_colum_minvals[0];
    }
        return m_colum_minvals[index];
}

double vpDataSet::column_max_value(ushort index)
{
    if (index >= m_colum_maxvals.count())
    {
        qDebug() << "[ERROR]vpDataSet::data_max_value-> indice fuera de rango:" << index;
        return m_colum_maxvals[0];
    }
        return m_colum_maxvals[index];
}

QString vpDataSet::info(bool to_screen)
{
    QString info;
    info.append("Name:\t").append(m_id).append("\n");
    info.append("Fields:\t").append(QString::number(m_data_fields.count(),'f',0)).append(" Rows:\t").append(QString::number(count_rows(),'f',0)).append("\n");

    for(int f = 0 ; f < m_data_fields.count() ; f++ )
    {
        QString field=m_data_fields[f];
        info.append("*").append(field).append("\n");
        info.append("\tmin:").append(QString::number(m_colum_minvals[f],'f',2)).append(" max:").append(QString::number(m_colum_maxvals[f],'f',2)).append(" avg:").append(QString::number(m_colum_average[f],'f',2)).append("\n");
    }

    if(to_screen)
    {
        qDebug() << info;
    }
    return info;
}

QString vpDataSet::column_info(ushort index, bool to_screen)
{
    QString info("-Field:\t");
    info.append(m_data_fields[index]);

    info.append("\nmin_val:\t");
    info.append(QString::number( m_colum_minvals[index],'f',2 ));

    info.append("\nmax_val:\t");
    info.append(QString::number( m_colum_maxvals[index],'f',2 ));
    info.append("\naverage:\t");
    info.append(QString::number( m_colum_average[index],'f',2 ));
    info.append("\n");

    if (to_screen)
    {
        qDebug() << info;
    }
    return info;
}

quint8  vpDataSet::indexOf(QString fieldname)
{
    for( int i = 0 ; i < m_data_fields.count() ; i++ )
    {
        if ( m_data_fields[i] == fieldname )
        {
            return i;
        }
    }
    qDebug() << "[ERROR]vpDataSet::get_id-> no encuentro el campo:" << fieldname;
    return -1;
}


bool vpDataSet::have_data_array()
{
    return ( (m_data_type == vpDataSet::datasetDoubleArray) ||
             (m_data_type == vpDataSet::datasetGpsArray) ||
             (m_data_type == vpDataSet::datasetTimedDoubleArray));
}

bool vpDataSet::have_timed_data()
{
    return ( (m_data_type == vpDataSet::datasetGpsArray) ||
             (m_data_type == vpDataSet::datasetGpsTrack) ||
             (m_data_type == vpDataSet::datasetTimedDoubleArray));
}

bool vpDataSet::have_gps_data()
{
    return ( (m_data_type == vpDataSet::datasetGpsTrack) ||
             (m_data_type == vpDataSet::datasetGpsArray));
}

//SLOTS

void vpDataSet::clear()
{
    m_rowsDoubles.clear();
    m_rowsStrings.clear();
    m_data_fields.clear();
    m_fields_unit.clear();
    m_fieldsColors.clear();
    m_columns.clear();
    m_colum_minvals.clear();
    m_colum_maxvals.clear();
    m_colum_average.clear();

    m_data_type=vpDataSet::datasetInvalid;
    emit data_updated();
}
