#ifndef VPDATASET_H
#define VPDATASET_H

#include <QObject>
#include <QVector>

#include "vpdatasource.h"

class vpDataSet : public QObject
{
    Q_OBJECT
public:
    explicit vpDataSet(QObject *parent = 0);
            ~vpDataSet();
    enum dataType
    {
        dataGenericCsv
    };

    bool             open(QString resource,vpDataSet::dataType = dataGenericCsv,bool full_load = true);
    void             initDataset(QString id,QVector<QString> fields,bool willStoreStrings =false);
    void             appendRowDoubles(QVector<double>  row,QVector<QString> fields = QVector<QString>());
    void             appendRowStrings(QVector<QString> row,QVector<QString> fields = QVector<QString>());
    QVector<double>  getRowDoubles(uint row);
    QVector<double>  getColumnDoubles(uint col);
    double           getPosDouble(uint row, uint col);

    QVector<QString> getRowStrings(uint row);
    QVector<QString> getColumnStrings(uint col);
    QString          getPosString(uint row, uint col);

    vpDataSet        getSubDataSet(QVector<QString> fields = QVector<QString>(),uint start = 0, uint count = 0);

    uint             getRowCount()      {if(m_storeInStrings) return m_dataStrs.count();else return m_dataDoubles.count();}
    uint             getColumnCount()   {return m_fields.count();}
    QVector<QString> getFields()        {return m_fields;}
    void             setId(QString n_id){m_id=n_id;}
    QString          getId()            {return m_id;}

protected:
    QString                     m_id;
    QVector<QString>            m_fields;
    bool                        m_storeInStrings;
    bool                        m_fullLoad;
    uint                        m_cacheStart;
    uint                        m_cacheCount;
    QVector<QVector<double> >   m_dataDoubles;
    QVector<QVector<QString> >  m_dataStrs;
    vpDataSource                m_dataSource;
signals:
    void dataUpdated();
private slots:

public slots:
    void load();
    void unload();
    void clear();
    void close();
};

#endif // VPDATASET_H
