#include "vpgraphwidget.h"

vpGraphWidget::vpGraphWidget(QWidget *parent) :
    QFrame(parent)
{
    m_viewport  = new vpGLES2Widget;
    m_graph     = new vp3DGeoGraph(QVector3D(0,0,0),QVector3D(10000,1000,0));
    m_fields_list = new QListWidget();
    m_ly_main = new QHBoxLayout(this);
    m_spacer0 = new QSpacerItem( 0, 0, QSizePolicy::MinimumExpanding,
                                       QSizePolicy::Minimum);
    m_spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding,
                                       QSizePolicy::Maximum);

    m_dataSource = 0;
    m_viewport->camera().set_movemode_arround(m_graph);

    m_viewport->add_geoObj(m_graph);
    m_ly_main->addLayout(&m_ly_vp_btns,5);
    m_ly_main->addWidget(m_fields_list,1);

    m_ly_vp_btns.addLayout(&m_ly_btns);
    m_ly_vp_btns.addWidget(m_viewport);

    m_ly_vp_btns.setSpacing(0);
    m_ly_main->setSpacing(0);
    m_ly_btns.setSpacing(0);
    m_ly_main->setContentsMargins(QMargins(1,1,1,1));
    this->setContentsMargins(QMargins(1,5,1,1));

    m_label0.setFont(QFont(m_label0.font().family(),8));
    m_label1.setFont(QFont(m_label1.font().family(),8));
    m_label0.setText("Graph type:");
    m_label1.setText("Graph type:");
    m_btn0.setMaximumSize(25,25);
    m_btn1.setMaximumSize(25,25);
    m_btn2.setMaximumSize(25,25);
    m_btn3.setMaximumSize(25,25);
    m_btn4.setMaximumSize(25,25);
    m_btn5.setMaximumSize(25,25);
    m_ly_btns.addWidget(&m_label0);
    m_ly_btns.addWidget(&m_btn0);
    m_ly_btns.addWidget(&m_btn1);
    m_ly_btns.addWidget(&m_btn2);
    m_ly_btns.addItem(m_spacer0);

    m_ly_btns.addWidget(&m_label1);
    m_ly_btns.addWidget(&m_btn3);
    m_ly_btns.addWidget(&m_btn4);
    m_ly_btns.addWidget(&m_btn5);
    m_ly_btns.addItem(m_spacer1);

    this->setFrameStyle(1);

    m_graph_populated = false;

    qRegisterMetaType<vpData>("vpData&");
    qRegisterMetaType<QVector<vpData> >("QVector<vpData>&");

    connect(m_fields_list,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(change_field_selection(QListWidgetItem*)));
}

vpGraphWidget::~vpGraphWidget()
{
    m_fields_list->clear();
    for ( int i = 0 ; i < m_fields_items.size() ; i++)
    {
        delete m_fields_items[i];
    }

    delete m_viewport;
    delete m_graph;
    delete m_fields_list;
    delete m_ly_main;
    delete m_spacer0;
    delete m_spacer1;
}

void vpGraphWidget::populate_listbox(QVector<QString> fields)
{
    m_fields_list->clear();
    m_fields_items.clear();
    for ( int i = 0 ; i < fields.count() ; i++ )
    {
        QListWidgetItem* n_item = new QListWidgetItem();
        m_fields_items.append(n_item);
        m_fields_items[i]->setText(fields[i]);
        m_fields_items[i]->setFlags( m_fields_items[i]->flags() |Qt::ItemIsUserCheckable);
        m_fields_items[i]->setCheckState(Qt::Unchecked);
        m_fields_items[i]->setBackgroundColor(Qt::white);
        m_fields_list->addItem(m_fields_items[i]);
    }
}

void vpGraphWidget::init_graph(vpDataContainer &datacontainer)
{
/*
    m_graph->clear();
    // Primero obtenemos los campos marcados y los indices que representan en el array de datos;
    QVector<QString> selected_fields;
    QVector<quint8> selected_indexs;
    for ( int i = 0 ; i < m_fields_items.size() ; i++ )
    {
        if ( m_fields_items[i]->checkState() == Qt::Checked )
        {
            selected_fields.append(m_fields_items[i]->text());
            selected_indexs.append(i);
        }
    }

//ahora rellenamos el grafico con los datos seleccionados
    for ( ushort j = 0 ; j < selected_indexs.count() ; j++ )
    {
        vpDataColumn data_column;
        data_column.field_name() = selected_fields[j];
        data_column.field_color() = m_fields_items[selected_indexs[j]]->background().color();
        for ( int i = 0 ; i < data_array.count() ; i++ )
        {
            data_column.add_data(data_array[i].to_qreal_vector()[selected_indexs[j]]);
        }
        m_graph->add_dataColumn(data_column);
    }
    m_graph_populated = true;
*/
}

void vpGraphWidget::append_data_to_graph(vpData &datarow)
{
/*
    QVector<QString> selected_fields;
    QVector<quint8> selected_indexs;
    for ( int i = 0 ; i < m_fields_items.size() ; i++ )
    {
        if ( m_fields_items[i]->checkState() == Qt::Checked )
        {
            selected_fields.append(m_fields_items[i]->text());
            selected_indexs.append(i);
        }
    }

    QVector<qreal> tmp_buffer = data.to_qreal_vector();
    QVector<qreal> data_row;
    for(quint8 i = 0 ; i < selected_fields.count() ; i++)
    {
        data_row.append(tmp_buffer[selected_indexs[i]]);
    }
    m_graph->add_dataRow(data_row);
*/
}

void vpGraphWidget::adquire_data(vpDataContainer &datacontainer)
{
    m_graph->clear_data();
    m_dataSource = &datacontainer;
    m_graph->set_dataContainer(m_dataSource);
    populate_listbox(datacontainer.get_fields());
}

void vpGraphWidget::change_field_selection(QListWidgetItem *item)
{//este slot recibe los eventos del listview
    //qDebug() << "checkstate cambiado:" << item->text() << item->checkState();
    m_fields_list->blockSignals(true);// hay que bloquear las señales para no generar un bocle infinito de secuencas de cambios
        if (item->checkState() == Qt::Checked)
        {
            quint8 r = qrand() % 220;
            quint8 g = qrand() % 220;
            quint8 b = qrand() % 220;
            QColor rnd_color(r,g,b);
            m_dataSource->color(item->text()) = rnd_color;
            item->setBackground(m_dataSource->color(item->text()));
        }
        else
        {
            m_dataSource->color(item->text()).setRgb(255,255,255);
            item->setBackground(m_dataSource->color(item->text()));
        }
    m_fields_list->blockSignals(false);
    QVector<QString> selected_fields;
    QVector<ushort> selected_indexs;
    for ( int i = 0 ; i < m_fields_items.size() ; i++ )
    {
        if ( m_fields_items[i]->checkState() == Qt::Checked )
        {
            selected_fields.append(m_fields_items[i]->text());
            selected_indexs.append(i);
        }
    }
    m_graph->set_selectedFields(selected_indexs);
}
