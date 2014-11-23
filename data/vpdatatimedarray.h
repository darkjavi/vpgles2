#ifndef VPDATATIMEDARRAY_H
#define VPDATATIMEDARRAY_H

#include "vpdataarraydoubles.h"

class vpDataTimedArray : public vpDataArrayDoubles
{
public:
    vpDataTimedArray(QString id = "none");
    virtual void initFields(QVector<QString> fields);
    virtual void append(const QVector<double> &row);
    virtual void setTimeField(quint32 index);
protected:
    quint32    m_timeField;
    bool       m_timeFieldFound;
    //elapse
};

#endif // VPDATATIMEDARRAY_H
