#include "vptestwindow.h"
#include "ui_vptestwindow.h"
#include "vpgles2widget.h"

vpTestWindow::vpTestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::vpTestWindow)
{
    ui->setupUi(this);

    vpGLES2Widget *m_viewport = new vpGLES2Widget(this);
    ui->verticalLayout->addWidget(m_viewport);

    vp3DGeoAxis* n_axis = new vp3DGeoAxis(QVector3D(0,0,0),QVector3D(100,100,100));
    n_axis->set_id("main_axis");
    m_viewport->add_geoObj(n_axis);



    QVector<QString> strings;
    strings.append("ele");
    strings.append("wele");
    strings.append("lerele");

    vp3DGeoGraph* n_graph = new vp3DGeoGraph(QVector3D(0,0,0),QVector3D(2000,1000,1));
    n_graph->set_id("test_graph");
    m_viewport->add_geoObj(n_graph);
    //m_camera.set_movemode_arround(n_graph);
    //n_graph->init_dataRows(strings);
    for ( int i = 0 ; i < 100 ; i++ )
    {
        QVector<qreal> data;
        data.append(qrand() % 1000);
        data.append(qrand() % 1000);
        data.append(qrand() % 1000);
        //n_graph->add_dataRow(data);
    }

}

vpTestWindow::~vpTestWindow()
{
    delete ui;
}
