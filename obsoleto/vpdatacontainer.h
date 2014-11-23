#ifndef VPDATACONTAINER_H
#define VPDATACONTAINER_H

#include <QObject>
#include <QColor>
#include <QFile>
#include <QString>
#include <QDateTime>

#include <vpdata.h>


#define DEFAULT_UPDATE_SIGNAL_THRESHOLD 1
#define DEFAULT_CSV_FIELD_DELIMITER     ","
#define DEFAULT_CSV_LINE_DELIMITER      "\n"

class vpDataContainer : public QObject
{
    Q_OBJECT
public:
    explicit vpDataContainer(QObject *parent = 0);
    vpDataContainer(const vpDataContainer& obj);

    QVector<vpData>&  data()        {return m_datarows;}
    QVector<QString>  get_fields()  {return m_fields;}
    ulong count()                   {return m_datarows.count();}

    void init_fields(QVector<QString> fields);

    QVector<qreal*>& column(QString fieldname);
    QVector<qreal*>& column(ushort index);

    QColor& color(QString fieldname);
    QColor& color(ushort index);

    qreal min_value(ushort index);
    qreal max_value(ushort index);

    QString info(bool to_screen);
    QString column_info(ushort index, bool to_screen = false);

    void appendRow(vpData dataRow);
    int indexOf(QString fieldname);
    void clear();

    bool csv_import(QString filename);
    bool csv_export(QString output_filename);
    bool csv_export(QString output_filename, QVector<ushort> selected_indexs);
    void set_csv_field_delimiter(QString d)     {m_csv_field_delimiter = d;}
    void set_csv_line_delimiter(QString d)      {m_csv_line_delimiter = d;}

protected:
    QVector<QString> m_fields;
    QVector<QColor>  m_colors;
    QVector<vpData>  m_datarows;
    QVector<QVector<qreal*> >   m_columns;
    QVector<qreal>              m_colum_minvals;
    QVector<qreal>              m_colum_maxvals;
    QVector<qreal>              m_colum_average;

    ushort  m_update_threshold;
    ushort  m_last_update;

    QString m_csv_field_delimiter;
    QString m_csv_line_delimiter;

    QByteArray generate_csv_header(QVector<ushort> field_index);
    QByteArray vpData2csv(vpData data);
    bool init_export_file(QString filename, QFile &resource, QVector<ushort> selected_fields);

signals:
    void data_updated();
    void data_cleared();
public slots:

};

#endif // VPDATACONTAINER_H
