#include "vpdatacontainer.h"

vpDataContainer::vpDataContainer(QObject *parent) :
    QObject(parent)
{
    m_update_threshold = DEFAULT_UPDATE_SIGNAL_THRESHOLD;
    m_last_update = 0;
    m_csv_field_delimiter = DEFAULT_CSV_FIELD_DELIMITER;
    m_csv_line_delimiter  = DEFAULT_CSV_LINE_DELIMITER;
}

vpDataContainer::vpDataContainer(const vpDataContainer &obj) :
    QObject(obj.parent())
{
    m_datarows  = obj.m_datarows;
    m_fields    = obj.m_fields;
    m_columns   = obj.m_columns;
    m_colors    = obj.m_colors;
    m_colum_minvals = obj.m_colum_minvals;
    m_colum_maxvals = obj.m_colum_maxvals;
    m_update_threshold = obj.m_update_threshold;
    m_last_update = m_last_update;
    m_csv_field_delimiter = obj.m_csv_field_delimiter;
    m_csv_line_delimiter  = obj.m_csv_line_delimiter;
}

QVector<qreal*>& vpDataContainer::column(QString fieldname)
{
    int fielindex = indexOf(fieldname);
    if ( fielindex != -1 )
    {
        return column(fielindex);
    }
    qDebug() << "[ERROR]vpDataContainer::column-> no encuentro el campo:" << fieldname << "devuelvo el primero..." ;
    return column(0);
}

QVector<qreal*>& vpDataContainer::column(ushort index)
{
    if (index >= m_columns.count())
    {
        qDebug() << "[ERROR]vpDataContainer::column-> indice fuera de rango:" << index << "se devuelve la primera columna";
//    }

        return m_columns[0];
    }
    else
    {
        return m_columns[index];
    }

}

QColor& vpDataContainer::color(QString fieldname)
{
    int fielindex = indexOf(fieldname);
    if ( fielindex != -1 )
    {
        return color(fielindex);
    }
    qDebug() << "[ERROR]vpDataContainer::color-> no encuentro el campo:" << fieldname ;
    return color(0);
}

QColor& vpDataContainer::color(ushort index)
{
    if (index >= m_colors.count())
    {
        qDebug() << "[ERROR]vpDataContainer::color-> indice fuera de rango:" << index;
        return m_colors[0];
    }
        return m_colors[index];
}

qreal vpDataContainer::min_value(ushort index)
{
    if (index >= m_colum_minvals.count())
    {
        qDebug() << "[ERROR]vpDataContainer::min_value-> indice fuera de rango:" << index;
        return m_colum_minvals[0];
    }
        return m_colum_minvals[index];
}

qreal vpDataContainer::max_value(ushort index)
{
    if (index >= m_colum_maxvals.count())
    {
        qDebug() << "[ERROR]vpDataContainer::max_value-> indice fuera de rango:" << index;
        return m_colum_maxvals[0];
    }
        return m_colum_maxvals[index];
}

QString vpDataContainer::info(bool to_screen)
{
    QString info;

    info.append("vpDataContainer::info->IMPLEMENTAME!!!");

    if(to_screen)
    {
        qDebug() << info;
    }
    return info;
}

QString vpDataContainer::column_info(ushort index, bool to_screen)
{
    QString info("-Field:\t");
    info.append(m_fields[index]);

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

void vpDataContainer::init_fields(QVector<QString> fields)
{
    clear();
    m_fields = fields;
    for ( int i = 0 ; i < m_fields.count() ; i++ )
    {
        QVector<qreal*> column;
        QColor          color(255,255,255);
        m_columns.append(column);
        m_colum_minvals.append(0);
        m_colum_maxvals.append(0);
        m_colum_average.append(0);
        m_colors.append(color);
    }
    emit data_updated();
}

void vpDataContainer::appendRow(vpData dataRow)
{
    if (dataRow.datarow().size() != m_fields.count())
    {
        qDebug()<< "[ERROR]vpDataContainer::appendRow-> tamaño de row inesperado:" << dataRow.datarow().size() << "esperado:" << m_fields.count();
        return;
    }
    m_datarows.append(dataRow);
    if ( m_datarows.size() == 1)
    {
        m_colum_minvals = m_datarows[0].datarow();
        m_colum_maxvals = m_datarows[0].datarow();
    }
    for ( int i = 0 ; i < dataRow.datarow().count() ; i++ )
    {
        m_columns[i].append(&m_datarows.last().datarow()[i]);

        m_colum_average[i] = ((m_colum_average[i] * (m_datarows.size()-1)) + dataRow.datarow()[i])/m_datarows.size();

        if ( dataRow.datarow()[i] < m_colum_minvals[i] )
        {
            m_colum_minvals[i] = dataRow.datarow()[i];
        }
        else if ( dataRow.datarow()[i] > m_colum_maxvals[i] )
        {
            m_colum_maxvals[i] = dataRow.datarow()[i];
        }
    }
    if (m_last_update >= m_update_threshold)
    {
        emit data_updated();
        m_last_update = 0;
    }
    else
    {
        m_last_update++;
    }
}

int vpDataContainer::indexOf(QString fieldname)
{
    for( int i = 0 ; i < m_fields.count() ; i++ )
    {
        if ( m_fields[i] == fieldname )
        {
            return i;
        }
    }
    qDebug() << "[ERROR]vpDataContainer::get_id-> no encuentro el campo:" << fieldname;
    return -1;
}

void vpDataContainer::clear()
{
    m_fields.clear();
    m_colors.clear();
    m_columns.clear();
    m_colum_minvals.clear();
    m_colum_maxvals.clear();
    emit data_cleared();
}

// import / export related
bool vpDataContainer::csv_import(QString filename)
{
    clear();
    QFile file(filename);
    if ( ! file.exists() )
    {
        qDebug() << "[ERROR]vpDataContainer::csv_import-> No existe el fichero:" << file.fileName() ;
        return false;
    }

    if ( ! file.open(QIODevice::ReadOnly) )
    {
        qDebug() << "[ERROR]vpDataContainer::csv_import-> No puedo abrir el fichero(permisos?):" << file.fileName() ;
        return false;
    }
    qDebug() << "[INFO]vpDataContainer::csv_import-> Abrierto:" << file.fileName() ;

    //Parsear aqui!!
    QString raw_data = file.readAll();
    QStringList datalines = raw_data.split(m_csv_line_delimiter);
    raw_data.clear();
//para no sobrecargar con los eventos de update, le aplicamos un reductor
    m_update_threshold = (int) (datalines.count() / 100) +1;

    QStringList fields = datalines.at(0).split(m_csv_field_delimiter);
    init_fields(fields.toVector());
    //rellenamos los datos;
    for ( int l = 1 ; l < datalines.count() ; l++ )
    {
        QStringList csv_datarow = datalines.at(l).split(m_csv_field_delimiter);
        QVector<qreal> datarow;
        for ( int f = 0 ; f < csv_datarow.count() ; f++ )
        {
            datarow.append(csv_datarow.at(f).toDouble());
        }
        appendRow(vpData(datarow));
    }
    qDebug() << "[INFO]vpDataContainer::csv_import-> Terminado:" << file.fileName() << "Procesadas :" << datalines.count() << "entradas" ;
    file.close();
    m_update_threshold = DEFAULT_UPDATE_SIGNAL_THRESHOLD;
    emit data_updated();
    m_last_update = 0;
    return true;
}

bool vpDataContainer::csv_export(QString output_filename)
{
    QVector<ushort> all_indexs;
    for ( int i = 0 ; i < m_fields.count() ; i++ )
    {
        all_indexs.append(i);
    }
    return csv_export(output_filename,all_indexs);
}

bool vpDataContainer::csv_export(QString output_filename, QVector<ushort> selected_indexs)
{
    QFile dump_file;
    if ( init_export_file(output_filename,dump_file,selected_indexs) )
    {
        dump_file.open(QIODevice::Append);
        for ( long i = 0 ; i < m_datarows.count() ; i++ )
        {
            QVector<qreal> tmp_data;
            for ( int s = 0 ; s < selected_indexs.count() ; s++ )
            {
                tmp_data.append(m_datarows[i].datarow()[selected_indexs[s]]);
            }
            dump_file.write(vpData2csv(vpData(tmp_data)));
        }
        dump_file.close();
        return true;
    }
    return false;
}

QByteArray vpDataContainer::generate_csv_header(QVector<ushort> field_index)
{
    QString header;
    for ( int i = 0 ; i < field_index.count() -1 ; i++ )
    {
        header.append(m_fields[field_index[i]]);
        header.append(m_csv_field_delimiter);
    }
    header.append(m_fields[field_index.last()]);
    header.append(m_csv_line_delimiter);
    QByteArray output;
    output.append(header);
    return output;
}

QByteArray vpDataContainer::vpData2csv(vpData data)
{
    QString data_line;

    for ( int i = 0 ; i < data.datarow().count() -1 ; i++ )
    {
        data_line.append(QString::number(data.datarow()[i],'f',4));
        data_line.append(m_csv_field_delimiter);
    }
    data_line.append(QString::number(data.datarow().last(),'f',4));
    data_line.append(m_csv_line_delimiter);

    QByteArray output;
    output.append(data_line);
    return output;
}

bool vpDataContainer::init_export_file(QString filename, QFile &resource , QVector<ushort> selected_fields )
{
    if ( resource.isOpen() )
    {
        resource.close();
    }

    filename.append("-");
    uint timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
    filename.append(QString::number(timestamp)); //timestamp en el nombre de fichero
    filename.append(".csv");
    resource.setFileName(filename);
    if ( resource.exists() )
    {
        qDebug() << "[WARN]vpDataContainer::init_export_file-> El fichero ya existe!" << resource.fileName();
        return false;
    }
    else
    {
        if ( ! resource.open(QIODevice::WriteOnly) )
        {
            qDebug() << "[ERROR]vpDataContainer::init_export_file-> No puedo crear el fichero!:" << resource.fileName();
            return false;
        }
        qDebug() << "[INFO]vpDataContainer::init_export_file-> creado fichero:" << resource.fileName();
        resource.write(generate_csv_header(selected_fields));
        resource.close();
        return true;
    }
}
