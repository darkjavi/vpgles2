#ifndef VPDATAPOOL_H
#define VPDATAPOOL_H

#include <QObject>

#include "vpdataset.h"

class vpDataPool : public QObject
{
    Q_OBJECT
public:
    explicit vpDataPool(QObject *parent = 0);
    void append_dataset(vpDataSet &n_dataset);
protected:
    QVector<vpDataSet> m_datasets;
signals:
    
public slots:
    void data_updated();
};

#endif // VPDATAPOOL_H
