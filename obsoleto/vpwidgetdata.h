#ifndef VPWIDGETDATA_H
#define VPWIDGETDATA_H

#include <QFrame>
#include <QComboBox>

#include "vpdatapool.h"

class vpWidgetData : public QFrame
{
    Q_OBJECT
public:
             vpWidgetData(vpDataPool* dataPool, QWidget *parent=0);
    explicit vpWidgetData(QWidget *parent = 0);
    ~vpWidgetData();
protected:
    QComboBox           m_dataPoolComboBox;
    vpDataPool*         m_dataPool;
    vpDataSet*          m_dataSet;
    bool                m_shared_dataPool;

    virtual void clear_data()   {return;}

signals:
    void new_dataPool(vpDataPool*);

private slots:
    void comboBoxHelper(int);
public slots:
    virtual void set_dataSet(vpDataSet* dataset);
    void         set_dataPool(vpDataPool* dataPool);
    void         dataPool_updated();
    virtual void dataSet_updated() = 0;
    void clear();
};

#endif // VPWIDGETDATA_H
