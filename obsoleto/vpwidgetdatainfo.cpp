#include "vpwidgetdatainfo.h"
#include "ui_vpwidgetdatainfo.h"

vpWidgetDataInfo::vpWidgetDataInfo(vpDataPool *dataPool, QWidget *parent) :
    vpWidgetData(dataPool,parent),
    ui(new Ui::vpWidgetDataInfo)
{
    ui->setupUi(this);
    ui->ly_info->addWidget(&m_dataPoolComboBox);
}

vpWidgetDataInfo::~vpWidgetDataInfo()
{
    delete ui;
}

void vpWidgetDataInfo::dataSet_updated()
{
    clear_data();
    if (m_dataSet)
    {
        QLabel* n_label = new QLabel();
        ui->ly_info->addWidget(n_label);
        m_info_labels.append(n_label);
        QString c_info;
        c_info.append("*CONTAINER_INFO:\n-Fields:\t\t");
        c_info.append(QString::number(m_dataSet->data()->fields().count(),'f',0));
        c_info.append("\n-Count:\t\t");
        c_info.append(QString::number(m_dataSet->data()->count(),'f',0));
        c_info.append("\n");
        n_label->setText(c_info);

        for ( int i = 0 ; i < m_dataSet->data()->fields().count() ; i++ )
        {
            QLabel* n_label = new QLabel();
            ui->ly_info->addWidget(n_label);
            m_info_labels.append(n_label);
            //n_label->setText(m_dataSet->column_info(i));
        }
    }
    else
    {
        QLabel* n_label = new QLabel(ui->scrollArea);
        m_info_labels.append(n_label);
        n_label->setText("NO DATA YET!");
    }
}

void vpWidgetDataInfo::clear_data()
{
    for ( int i = 0 ; i < m_info_labels.count() ; i++ )
    {
        QLabel* label = m_info_labels[i];
        delete label;
    }
    m_info_labels.clear();
}
