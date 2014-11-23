#ifndef VPDATAARRAY_H
#define VPDATAARRAY_H

#include "vpdata.h"
#include <QColor>

class vpDataArrayDoubles : public vpData
{
public:
    vpDataArrayDoubles(QString id = "none");

    quint64             count(){return m_rows.count();}
    void                initFields(QVector<QString> fields);
    QVector<double>     rowToDoubles(int row);
    QVector<QString>    rowToStrings(int row);
    QVector<double>     columnToDoubles(int col);
    QVector<QString>    columnToStrings(int col);
    QString             posToString(quint32 col,quint32 row);
    double              posToDouble(quint32 col,quint32 row);
    void                append(const QVector<double>& row);
    void                append(QVector<QString>& row);
    double              columnMinVal(quint32 col);
    double              columnMaxVal(quint32 col);
    double              columnAvgVal(quint32 col);
protected:
    void processRow(QVector<double>& row);

    QVector<QVector<double> >    m_rows;
    QVector<QVector<double*> >   m_columns;
    QVector<double>              m_columnMinVals;
    QVector<double>              m_columnMaxVals;
    QVector<double>              m_columnAverage;
};

#endif // VPDATAARRAY_H
