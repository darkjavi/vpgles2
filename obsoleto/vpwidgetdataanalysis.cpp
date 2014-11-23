#include "vpwidgetdataanalysis.h"
#include "ui_vpwidgetdataanalysis.h"

vpWidgetDataAnalysis::vpWidgetDataAnalysis(vpDataPool *dataPool, QWidget *parent) :
    vpWidgetData(dataPool,parent),
    ui(new Ui::vpWidgetDataAnalysis)
{
    ui->setupUi(this);

    for ( int i = 0 ; i < 2 ; i++ )
    {
        add_viewport();
    }
    m_stats_panel = new vpWidgetDataInfo(m_dataPool,this);
    m_dataPoolWidget = new vpDataPoolWidget(m_dataPool,this);
    ui->tab1_ly->addWidget(m_stats_panel);
    ui->ly_btns->addWidget(m_dataPoolWidget);

    connect(ui->btn_add_viewport,SIGNAL(released()),this,SLOT(add_viewport()));
    connect(ui->btn_remove_viewport,SIGNAL(released()),this,SLOT(remove_viewport()));
}

vpWidgetDataAnalysis::~vpWidgetDataAnalysis()
{
    delete ui;
}

void vpWidgetDataAnalysis::add_viewport()
{
    if ( m_viewports.count() == 0 )
    {//si en este punto count() vale 0 es que es el primer vp que añadimos
        ui->btn_remove_viewport->setEnabled(true);
    }

    vpWidgetGraph* n_vp = new vpWidgetGraph(m_dataPool,this);

    QString title("Graph #");
    title.append(QString::number(m_viewports.count()));
    n_vp->set_title(title);
    m_viewports.append(n_vp);
    ui->layout_vps->addWidget(n_vp);

    connect(this,SIGNAL(new_dataPool(vpDataPool*)),
            n_vp,SLOT(set_dataPool(vpDataPool*)));

}

void vpWidgetDataAnalysis::remove_viewport()
{
    if(m_viewports.count() > 0)
    {
        vpWidgetGraph* vp = m_viewports[m_viewports.count()-1];
        delete vp;
        m_viewports.remove(m_viewports.count()-1);
    }
    if ( m_viewports.count() == 0 )
    {
        ui->btn_remove_viewport->setEnabled(false);
    }
}
