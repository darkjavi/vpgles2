#include "vpdockwidgettest.h"
#include "ui_vpdockwidgettest.h"

vpDockWidgetTest::vpDockWidgetTest(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::vpDockWidgetTest)
{
    ui->setupUi(this);
    m_glwidget = new vpGLES2Widget();
    QHBoxLayout* vp_ly = new QHBoxLayout;
    vp_ly->setSpacing(0);
    ui->frame_vp->setContentsMargins(QMargins(0,0,0,0));
    ui->frame_vp->setLayout(vp_ly);
    ui->frame_vp->layout()->addWidget(m_glwidget);

    m_glwidget->camera().set_position(QVector3D(10,10,100));

    m_benchmarking = false;

    connect(ui->btn_bench,SIGNAL(clicked()),this,SLOT(benchmark()));

}

vpDockWidgetTest::~vpDockWidgetTest()
{
    delete ui;
    delete m_glwidget;
}


//SLOTS

void vpDockWidgetTest::benchmark()
{
    if (m_benchmarking)
    {
        ui->btn_bench->setText("Benchmark");
        m_glwidget->stop_benchmark();
        disconnect(m_glwidget,SIGNAL(performance_info(ulong,ulong,ulong,ulong,ulong)),this,SLOT(collect_benchmark_info(ulong,ulong,ulong,ulong,ulong)));
        m_benchmarking = false;
    }
    else
    {
        ui->btn_bench->setText("Benchmarking");
        m_glwidget->start_benchmark();
        connect(m_glwidget,SIGNAL(performance_info(ulong,ulong,ulong,ulong,ulong)),this,SLOT(collect_benchmark_info(ulong,ulong,ulong,ulong,ulong)));
        m_benchmarking =true;
    }
}

void vpDockWidgetTest::collect_benchmark_info(ulong objs,ulong components,ulong vertex,ulong scene_time,ulong vp_time)
{

}
