#include "vpdatapool.h"

vpDataPool::vpDataPool(QObject *parent) :
    QObject(parent)
{
}

void vpDataPool::append_dataset(vpDataSet &n_dataset)
{
    m_datasets.append(n_dataset);
    emit data_updated();
}


void vpDataPool::data_updated()
{

}
