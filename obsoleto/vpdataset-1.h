#ifndef VPDATASET_H
#define VPDATASET_H

#include <QObject>
#include <QColor>

#include <QDebug>

////////////////////////////////////////////
//La estructura esta planificada para que trabaje con arrays de string o mixtos double/string
//  pero no esta del todo lista!!!! TODO++
////////////////////////////////////////////
class vpDataSet : public QObject
{
    Q_OBJECT
public:
    enum datasetType
    {
        datasetInvalid,
        datasetDoubleArray,
        datasetTimedDoubleArray,
        datasetGpsTrack,
        datasetGpsArray
    };
    explicit vpDataSet(QVector<QString> fields, QString id = "unamed-data", quint8 dataType = vpDataSet::datasetDoubleArray);
    vpDataSet(const vpDataSet& obj);
    void operator=(const vpDataSet& obj);

    void append(QVector<double>   row_doubles,QVector<QString> row_strs);
    void append(QVector<double>   row_doubles);
    void append(QVector<QString>  row_strs);

    QString id()                                      {return m_id;}
    const QVector<double>&   getRowDoubles(int index) {if(index>m_rowsDoubles.count()) {qDebug()<<"vpDataset::getRow-<out of index!"<<index;index=0;}return m_rowsDoubles[index];}
    const QVector<double>&   lastRowDoubles()         {return m_rowsDoubles.last();}
    const QVector<QString>&  getRowStrings(int index) {if(index>m_rowsStrings.count()) {qDebug()<<"vpDataset::getRow-<out of index!"<<index;index=0;}return m_rowsStrings[index];}
    QVector<QString>  getFields()                     {return m_data_fields;}
    uint              count_rows()                    {return m_rowsDoubles.count();}//Ojo a cuando funcione en modo string!!!!
    uint              count_columns()                 {return m_data_fields.count();}

    QString getFieldUnit(int index)
    {
        if(index>m_fields_unit.count())
        {qDebug()<<"vpDataset::getFieldUnit-<out of index!"<<index;index=0;
            return QString("error");
        }else {return m_fields_unit[index];}
    }
    QVector<double*>& column(QString fieldname);
    QVector<double*>& column(ushort index);
    QString           column_info(ushort index, bool to_screen = false);
    double            column_min_value(ushort index);
    double            column_max_value(ushort index);
    QColor            get_fieldColor(int index);
    void              set_fieldColor(int index,QColor n_color);

    QString info(bool to_screen = false);

    quint8 indexOf(QString fieldname);

    bool have_data_array();
    bool have_timed_data();
    bool have_gps_data();

protected:
    QString                      m_id;
    QVector<QVector<double> >    m_rowsDoubles;
    QVector<QVector<QString> >   m_rowsStrings;
    QVector<QString>             m_data_fields;
    QVector<QString>             m_fields_unit;
    QVector<QColor>              m_fieldsColors;
    QVector<QVector<double*> >   m_columns;
    QVector<double>              m_colum_minvals;
    QVector<double>              m_colum_maxvals;
    QVector<double>              m_colum_average;
    qint32                       m_timeField;
    qint32                       m_latitudeField;
    qint32                       m_longitudeField;
    qint32                       m_altitudeField;
    qint32                       m_gpsacurracyField;

    quint8 m_data_type;

    void init_data_fields(QVector<QString> data_fields);
    void append_to_stats(QVector<double> &last);
    void reset_stats();
signals:
    void data_updated();
    void new_data(QVector<double>&);
public slots:
    void clear();
};

#endif // VPDATASET_H
