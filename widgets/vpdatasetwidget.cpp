#include "vpdatasetwidget.h"
#include "ui_vpdatasetwidget.h"
#include <QMessageBox>
#include <QFileDialog>

vpDataSetWidget::vpDataSetWidget(vpDataSet *dataset, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::vpDataSetWidget)
{
    ui->setupUi(this);
    setDataSet(dataset);
}

vpDataSetWidget::vpDataSetWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::vpDataSetWidget)
{
    ui->setupUi(this);
}

void vpDataSetWidget::setDataSet(vpDataSet *d)
{
    m_dataset=d;
    ui->dataSourceWidget->setDataSource(d->dataSource());
    connect(m_dataset,SIGNAL(destroyed()),this,SLOT(datasetDeleted()));
    connect(m_dataset,SIGNAL(updated()),this,SLOT(datasetUpdated()));
    datasetUpdated();
}

vpDataSetWidget::~vpDataSetWidget()
{
    delete ui;
}

void vpDataSetWidget::on_btn_delete_clicked()
{
    if(QMessageBox::Yes == QMessageBox::question(this, "Danger,Will robinson!", "Are you fucking sure?", QMessageBox::Yes|QMessageBox::No))
    {
        m_dataset->deleteLater();
    }
}

void vpDataSetWidget::datasetDeleted()
{
    this->deleteLater();
}

void vpDataSetWidget::datasetUpdated()
{
    ui->edit_id->setText(m_dataset->getId());
}

void vpDataSetWidget::on_edit_id_returnPressed()
{
    m_dataset->setId(ui->edit_id->text());
}

void vpDataSetWidget::on_edit_id_editingFinished()
{
    ui->edit_id->setText(m_dataset->getId());
}
