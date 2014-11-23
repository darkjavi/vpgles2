#include "vpdatasourcewidget.h"
#include "ui_vpdatasourcewidget.h"

vpDataSourceWidget::vpDataSourceWidget(vpDataSource* parent) :
    QWidget(),
    ui(new Ui::vpDataSourceWidget)
{
    ui->setupUi(this);
    m_dataSource = parent;
}

vpDataSourceWidget::~vpDataSourceWidget()
{
    delete ui;
}

void vpDataSourceWidget::add_widget(QWidget *widget)
{
    ui->ly_btns->addWidget(widget);
}

void vpDataSourceWidget::on_btn_addFile_clicked()
{
    QFileDialog d;
    if(d.exec())
    {
        m_dataSource->add_parserCSV(d.selectedFiles().at(0));
    }
}
