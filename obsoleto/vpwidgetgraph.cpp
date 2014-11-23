#include "vpwidgetgraph.h"
#include "ui_vpwidgetgraph.h"


vpWidgetGraph::vpWidgetGraph(vpDataPool *dataPool, QWidget *parent) :
    vpWidgetData(dataPool,parent),
    ui(new Ui::vpWidgetGraph)
{
    ui->setupUi(this);
    m_gb_x_field = 0;
    m_cb_x_field = 0;
    m_gb_z_field = 0;
    m_cb_z_field = 0;
    m_viewport  = new vpGLES2Widget(ui->frame_viewport);
    ui->dataset_gb->layout()->addWidget(&m_dataPoolComboBox);
    //ui->frame_viewport->setLayoutDirection(Qt::Horizontal);
    ui->ly_viewport->addWidget(m_viewport);
    m_graph     = new vp3DGeoGraph(QVector3D(0,0,0),QVector3D(10000,1000,0));
    //vp3DGeoAxis* axis = new vp3DGeoAxis(QVector3D(0,0,0),QVector3D(1000,1000,500));
    //vp3DGeoPrimitive* sphere = new vp3DGeoPrimitive(vp3DGeoPrimitive::PrimitiveSphere ,QVector3D(0,0,0),QVector3D(500,0,0),QColor(254,0,0));
    //m_viewport->scene().add_ojb(sphere);

    m_viewport->camera().set_movemode_along(m_graph,QVector3D(0,0,1));
    //m_viewport->camera().set_movemode_along(sphere,QVector3D(0,0,1));
    //m_viewport->camera().focus_object(sphere);
    m_viewport->scene().add_ojb(m_graph);
    //m_viewport->scene().add_ojb(axis);
    //m_viewport->camera().set_shuttler_mode_auto();
    //m_viewport->camera().set_shuttler_maxfps(200);

    //qRegisterMetaType<vpData>("vpData&");
    //qRegisterMetaType<QVector<vpData> >("QVector<vpData>&");

    connect(ui->listWidget_y_fields,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(change_field_selection(QListWidgetItem*)));
    connect(ui->btn_export_csv,SIGNAL(released()),this,SLOT(export_selected_csv()));
    connect(ui->btn_graph1d,SIGNAL(clicked()),this,SLOT(set_graph_mode1d()));
    connect(ui->btn_graph2d,SIGNAL(clicked()),this,SLOT(set_graph_mode2d()));
    connect(ui->btn_graph3d,SIGNAL(clicked()),this,SLOT(set_graph_mode3d()));
    connect(ui->btn_scale_abs,SIGNAL(clicked()),m_graph,SLOT(set_scale_absolute()));
    connect(ui->btn_scale_rel,SIGNAL(clicked()),m_graph,SLOT(set_scale_relative()));
    dataSet_updated();
}

vpWidgetGraph::~vpWidgetGraph()
{
    delete ui;
    //clear_list_and_items(); //peta! creo que el delete ui ya borra parte de las cosas
    delete m_viewport;
    delete m_graph;
    if(m_gb_x_field)
    {
        delete m_cb_x_field;
        delete m_gb_x_field;
    }
    if(m_gb_z_field)
    {
        delete m_cb_z_field;
        delete m_gb_z_field;
    }
}

void vpWidgetGraph::set_title(QString title, QFont font)
{
    ui->label_title->setText(title);
    ui->label_title->setFont(font);
}

void vpWidgetGraph::populate_listbox(vpDataSet *dataset)
{
    QVector<QString> last_selected = get_selected_fields();
    ui->listWidget_y_fields->clear();
    m_fields_items.clear();
    for ( int i = 0 ; i < dataset->data()->fields().count() ; i++ )
    {
        QListWidgetItem* n_item = new QListWidgetItem();
        m_fields_items.append(n_item);
        m_fields_items[i]->setText(dataset->data()->fields()[i]);
        m_fields_items[i]->setFlags( m_fields_items[i]->flags() |Qt::ItemIsUserCheckable);
        if(last_selected.contains(n_item->text()))
        {
            m_fields_items[i]->setCheckState(Qt::Checked);
        }
        else
        {
            m_fields_items[i]->setCheckState(Qt::Unchecked);
        }
        m_fields_items[i]->setBackgroundColor(dataset->data()->fieldColor(i));
        ui->listWidget_y_fields->addItem(m_fields_items[i]);
    }
}

void vpWidgetGraph::clear_list_and_items()
{
    ui->listWidget_y_fields->clear();
    m_fields_items.clear();
    //    for ( int i = 0 ; i < m_fields_items.size() ; i++)
//    {
        //QListWidgetItem* item = m_fields_items[i];
        //delete item;// parece ser q el listwidget ya los borra
//    }
}

QVector<ushort> vpWidgetGraph::get_selected_indexs()
{
    QVector<ushort> selected_indexs;
    for ( int i = 0 ; i < m_fields_items.size() ; i++ )
    {
        if ( m_fields_items[i]->checkState() == Qt::Checked )
        {
            selected_indexs.append(i);
        }
    }
    return selected_indexs;
}

QVector<QString> vpWidgetGraph::get_selected_fields()
{
    QVector<QString> selected_fields;
    for ( int i = 0 ; i < m_fields_items.size() ; i++ )
    {
        if ( m_fields_items[i]->checkState() == Qt::Checked )
        {
            selected_fields.append(m_fields_items[i]->text());
        }
    }
    return selected_fields;
}

void vpWidgetGraph::change_field_selection(QListWidgetItem *item)
{//este slot recibe los eventos del listview
    //qDebug() << "checkstate cambiado:" << item->text() << item->checkState();
    ui->listWidget_y_fields->blockSignals(true);// hay que bloquear las señales para no generar un bocle infinito de secuencas de cambios
        if (item->checkState() == Qt::Checked)
        {
            quint8 r = qrand() % 220;
            quint8 g = qrand() % 220;
            quint8 b = qrand() % 220;
            QColor rnd_color(r,g,b);
            m_dataSet->data()->setFieldColor(m_dataSet->data()->indexOf(item->text()),rnd_color);
        }
        else
        {
            m_dataSet->data()->setFieldColor(m_dataSet->data()->indexOf(item->text()),Qt::white);
        }
    ui->listWidget_y_fields->blockSignals(false);
    QVector<ushort> selected_indexs = get_selected_indexs();
    m_graph->set_selectedFields(selected_indexs);
    if (selected_indexs.count() > 1)
    {
        ui->btn_export_csv->setEnabled(true);
    }
    else
    {
        ui->btn_export_csv->setEnabled(false);
    }
}

void vpWidgetGraph::clear_data()
{
    clear_list_and_items();
    ui->btn_export_csv->setEnabled(false);
}


void vpWidgetGraph::set_dataSet(vpDataSet *dataset)
{
    vpWidgetData::set_dataSet(dataset);
    m_graph->set_dataset(dataset);
}

void vpWidgetGraph::dataSet_updated()
{
    if(m_dataSet)
    {
        populate_listbox(m_dataSet);
    }
    else
    {
        clear_list_and_items();
    }
}

void vpWidgetGraph::export_selected_csv()
{
    QFileDialog file_selection;
    file_selection.setFileMode(QFileDialog::AnyFile);
    file_selection.setWindowTitle("Export selected fields ($TIMESTAMP.csv will be appended!)");
    file_selection.setNameFilter("*.csv");
    if ( file_selection.exec())
    {
/*
        if (m_dataset->csv_export(file_selection.selectedFiles()[0],get_selected_indexs()))
        {
            ui->btn_export_csv->setPalette(QPalette(Qt::green));
            ui->btn_export_csv->setAutoFillBackground(true);
        }
        else
        {
            ui->btn_export_csv->setPalette(QPalette(Qt::red));
            ui->btn_export_csv->setAutoFillBackground(true);
        }
*/
    }
}

void vpWidgetGraph::set_graph_mode1d()
{
    qDebug() << "1D MODE";
    if(m_gb_x_field)
    {
        delete m_cb_x_field;
        delete m_gb_x_field;
        m_gb_x_field    = 0;
        m_cb_x_field    = 0;
    }
    if(m_gb_z_field)
    {
        delete m_cb_z_field;
        delete m_gb_z_field;
        m_gb_z_field    = 0;
        m_cb_z_field    = 0;
    }
}

void vpWidgetGraph::set_graph_mode2d()
{
    qDebug() << "2D MODE";
    if(m_gb_z_field)
    {
        delete m_cb_z_field;
        delete m_gb_z_field;
        m_gb_z_field    = 0;
        m_cb_z_field    = 0;
    }
    if ( ! m_gb_x_field )
    {
        m_gb_x_field = new QGroupBox();
        m_gb_x_field->setTitle("x_field");
        m_gb_x_field->setFont(QFont("Arial",8));
        m_gb_x_field->setContentsMargins(QMargins(0,0,0,0));
        ui->frame_side->layout()->addWidget(m_gb_x_field);
        m_cb_x_field = new QComboBox();
        m_gb_x_field->setLayout(new QHBoxLayout());
        m_gb_x_field->layout()->setContentsMargins(QMargins(0,0,0,0));
        m_gb_x_field->layout()->addWidget(m_cb_x_field);
    }
}

void vpWidgetGraph::set_graph_mode3d()
{
    qDebug() << "3D MODE";
    if ( ! m_gb_x_field )
    {
        m_gb_x_field = new QGroupBox();
        m_gb_x_field->setTitle("x_field");
        m_gb_x_field->setFont(QFont("Arial",8));
        m_gb_x_field->setContentsMargins(QMargins(0,0,0,0));
        ui->frame_side->layout()->addWidget(m_gb_x_field);
        m_cb_x_field = new QComboBox();
        m_gb_x_field->setLayout(new QHBoxLayout());
        m_gb_x_field->layout()->setContentsMargins(QMargins(0,0,0,0));
        m_gb_x_field->layout()->addWidget(m_cb_x_field);
    }
    m_gb_z_field = new QGroupBox();
    m_gb_z_field->setTitle("z_field");
    m_gb_z_field->setFont(QFont("Arial",8));
    m_gb_z_field->setContentsMargins(QMargins(0,0,0,0));
    ui->frame_side->layout()->addWidget(m_gb_z_field);
    m_cb_z_field = new QComboBox();
    m_gb_z_field->setLayout(new QHBoxLayout());
    m_gb_z_field->layout()->setContentsMargins(QMargins(0,0,0,0));
    m_gb_z_field->layout()->addWidget(m_cb_z_field);
}
