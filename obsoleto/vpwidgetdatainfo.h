#ifndef VPWIDGETDATAINFO_H
#define VPWIDGETDATAINFO_H

#include <QFrame>
#include <QLabel>

#include "vpwidgetdata.h"

namespace Ui {
class vpWidgetDataInfo;
}

class vpWidgetDataInfo : public vpWidgetData
{
    Q_OBJECT
    
public:
    explicit vpWidgetDataInfo(vpDataPool* dataPool = 0, QWidget *parent = 0);
    ~vpWidgetDataInfo();
    
private:
    Ui::vpWidgetDataInfo *ui;
    QVector<QLabel*>    m_info_labels;
    void clear_data();
public slots:
    void dataSet_updated() ;
};

#endif // VPWIDGETDATAINFO_H
