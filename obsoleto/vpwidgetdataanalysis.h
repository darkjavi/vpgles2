#ifndef VPWIDGETDATAANALYSIS_H
#define VPWIDGETDATAANALYSIS_H


#include "vpwidgetdata.h"
#include "vpdatapoolwidget.h"
#include "vpwidgetgraph.h"
#include "vpwidgetdatainfo.h"


namespace Ui {
class vpWidgetDataAnalysis;
}

class vpWidgetDataAnalysis : public vpWidgetData
{
    Q_OBJECT
    
public:
    explicit vpWidgetDataAnalysis(vpDataPool* dataPool = 0, QWidget *parent = 0);
    ~vpWidgetDataAnalysis();    
protected:
    Ui::vpWidgetDataAnalysis *ui;
    vpDataPoolWidget         *m_dataPoolWidget;
    QVector<vpWidgetGraph*>   m_viewports;
    vpWidgetDataInfo         *m_stats_panel;

signals:

private slots:
    void add_viewport();
    void remove_viewport();
public slots:
    void dataSet_updated() {return;}
};

#endif // VPWIDGETDATAANALYSIS_H
