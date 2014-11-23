#ifndef VPDATASET_H
#define VPDATASET_H

#include <QObject>
#include <QVector>

#include "vpdatasource.h"

class vpDataSource;

class vpDataSet : public QObject
{
    Q_OBJECT
public:
    explicit vpDataSet(QObject *parent = 0);
            ~vpDataSet();
    vpDataSet(const vpDataSet& obj);
    void operator =(const vpDataSet& obj);
    enum dataType
    {
        dataGenericCsv
    };

    bool             open(QString resource,bool full_load = true);
    void             initDataset(QString id,QVector<QString> fields,bool willStoreStrings =false);
    int              indexOf(QString field);
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
    void             setId(QString n_id){m_id=n_id;emit updated();}
    QString          getId()            {return m_id;}

    vpDataSet*       auxDataSet()       {return m_auxDataSet;}
    void             enableAuxDataset() {if(!m_auxDataSet) m_auxDataSet = new vpDataSet();}

    QVector<QVector<double> >&      dataDoubles() {return m_dataDoubles;}
    QVector<QVector<QString> >&     dataStrings() {return m_dataStrs;}
    vpDataSource*                   dataSource()  {return m_dataSource;}

    bool                            isStorageInStrings()    {return m_storeInStrings;}
protected:
    vpDataSet*                  m_auxDataSet;
    vpDataSource*               m_dataSource;
    QString                     m_id;
    QVector<QString>            m_fields;
    bool                        m_storeInStrings;
    bool                        m_fullLoad;
    uint                        m_cacheStart;
    uint                        m_cacheCount;
    QVector<QVector<double> >   m_dataDoubles;
    QVector<QVector<QString> >  m_dataStrs;
signals:
    void updated();
private slots:

public slots:
    void load();
    void unload();
    void clear();
    void    appendRowDoubles(const QVector<double>  row,QVector<QString> fields = QVector<QString>());
    void    appendRowStrings(const QVector<QString> row,QVector<QString> fields = QVector<QString>());
    void    appendDataDoubles(const QVector<QVector<double> >  data, QVector<QString> fields = QVector<QString>());
    void    appendDataStrings(const QVector<QVector<QString> > data, QVector<QString> fields = QVector<QString>());
};

#endif // VPDATASET_H
