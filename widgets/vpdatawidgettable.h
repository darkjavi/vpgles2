#ifndef VPDATAWIDGETTABLE_H
#define VPDATAWIDGETTABLE_H

#include "vpdatawidget.h"
#include <QTableWidget>

class vpDataWidgetTable : public vpDataWidget
{
    Q_OBJECT
public:
    explicit vpDataWidgetTable(vpDataPool* datapool, QWidget *parent = 0);
protected:
    QTableWidget m_table;
signals:
    
public slots:
    void refresh();
};

#endif // VPDATAWIDGETTABLE_H
