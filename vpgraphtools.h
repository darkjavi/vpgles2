#ifndef VPGRAPHTOOLS_H
#define VPGRAPHTOOLS_H

#include "vpdatapool.h"
class vpGraphTools
{
    static QVector<QVector<QVector2D> > gen2DGraph(QVector<vpDataSet*> datasets,QVector<QString> selected_fields, quint32 x_size= 400,quint32 y_size=100,quint32 start = 0,quint32 maxlength = 1000)
    {
        QVector<QVector<QVector2D> > result;
        for(int i = 0 ; i < selected_fields.count() ; i++)
        {
            result.append(QVector<QVector<QVector2D> >);
        }
        int d=0;
        int r=0;
        for(int i = 0 ; i < start ; i++)
        {
            if(r >= datasets[startd]->data()->count())
            {
                d++;
                r=0;
            }
            if(d >= datasets.count())
            {
                return;
            }
            r++;
        }
        for(int f = 0 ; f < selected_fields.count() ; f++)
        {
        int fieldIndex = datasets[d]->data()->indexOf(selected_fields[f]);

        for(int i = 0 ; i < maxlength ; i++)
        {

            if(r >= datasets[d]->data()->count())
            {
                d++;
                r=0;
            }
            if(d >= datasets.count())
            {
                break;
            }
        }

    }
};


#endif // VPGRAPHTOOLS_H
