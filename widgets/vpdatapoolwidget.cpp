#include "vpdatapoolwidget.h"
#include "ui_vpdatapoolwidget.h"
#include <QFileDialog>

vpDataPoolWidget::vpDataPoolWidget(vpDataPool *dataPool, QWidget *parent):
    QGroupBox(parent),
    ui(new Ui::vpDataPoolWidget)
{
    ui->setupUi(this);
    m_dataPool = dataPool;
    m_shared_dataPool = true;
    connect(ui->btn_addDataSet,SIGNAL(clicked()),m_dataPool,SLOT(addEmptyDataSet()));
    connect(m_dataPool,SIGNAL(updated()),this,SLOT(populateList()));
    populateList();
}

vpDataPoolWidget::vpDataPoolWidget(QWidget *parent):
    QGroupBox(parent),
    ui(new Ui::vpDataPoolWidget)
{
    ui->setupUi(this);
    m_dataPool = new vpDataPool;
    m_shared_dataPool = false;
    connect(ui->btn_addDataSet,SIGNAL(clicked()),m_dataPool,SLOT(addEmptyDataSet()));
    connect(m_dataPool,SIGNAL(updated()),this,SLOT(populateList()));
    populateList();
}

vpDataPoolWidget::~vpDataPoolWidget()
{
    delete ui;
    if(!m_shared_dataPool)
    {
        delete m_dataPool;
    }
}

void vpDataPoolWidget::populateList()
{
    ui->list_dataSets->clear();
    for(int i = 0 ; i < m_dataPool->count(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem(m_dataPool->dataSets()[i]->getId());
        vpDataSetWidget* widget = new vpDataSetWidget(m_dataPool->dataSets()[i]);
        ui->list_dataSets->addItem(item);
        ui->list_dataSets->setItemWidget(item,widget);
    }
    this->setTitle("DataPool-"+QString::number(m_dataPool->count())+"Files");
}

void vpDataPoolWidget::on_btn_importCSV_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select CSV");
    if(filename.isEmpty())return;
    m_dataPool->importCSV(filename);
}

void vpDataPoolWidget::on_btn_importNMEA_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select NMEA");
    if(filename.isEmpty())return;
    m_dataPool->importNMEA(filename);
}

void vpDataPoolWidget::on_btn_importTorque_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select TORQUE");
    if(filename.isEmpty())return;
    m_dataPool->importTorqueLog(filename);
}

void vpDataPoolWidget::on_btn_recursiveImportCSV_clicked()
{

}

void vpDataPoolWidget::on_btn_recursiveImportNMEA_clicked()
{
    QFileDialog dir_dialog;
    dir_dialog.setFileMode(QFileDialog::Directory);
    if (dir_dialog.exec())
    {
        m_dataPool->recursiveImportNMEA(dir_dialog.selectedFiles().first());
    }
}

void vpDataPoolWidget::on_btn_recursiveImportTorque_clicked()
{

}

void vpDataPoolWidget::on_btn_connectGpsd_clicked()
{

}

void vpDataPoolWidget::on_btn_connectMF_clicked()
{

}
