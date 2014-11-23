#include "vpparserwidget.h"
#include "ui_vpparserwidget.h"

#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>

vpParserWidget::vpParserWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::vpParserWidget)
{
    ui->setupUi(this);
    connect(ui->btn_exportCSV,SIGNAL(clicked()),this,SLOT(open_exportDialog()));
    ui->tx_widget->set_background_color(QColor(Qt::black));
}

vpParserWidget::~vpParserWidget()
{
    delete ui;
}

void vpParserWidget::set_parser(vpParser *parser)
{
    m_parser=parser;
    connect(parser,SIGNAL(dataTx()),ui->tx_widget,SLOT(tx()));
    connect(parser,SIGNAL(dataRx()),ui->tx_widget,SLOT(rx()));
    connect(parser,SIGNAL(progress(quint8)),ui->tx_widget,SLOT(set_progress(quint8)));
    connect(parser,SIGNAL(error()),ui->tx_widget,SLOT(error()));
    //connect(parser,SIGNAL(destroyed()),this,SLOT(deleteLater()));
    //ui->label_source->setText(parser->source().right(20));
    //ui->label_source->setToolTip(parser->source());

}

void vpParserWidget::open_exportDialog()
{
//    QToolButton* b=(QToolButton*)sender();
    QString filter="CSV("+m_parser->source()+".csv)";
    QString filename=QFileDialog::getSaveFileName(this,"Save file",QString(),filter);
    if(filename.isEmpty())return;
    if(!filename.endsWith(".csv"))filename.append(".csv");
    //m_parser->exportCSV(filename,true);
        //b->setco ("OK!");
    //else
        //b->setText("Error!");
}

void vpParserWidget::on_btn_info_clicked()
{
    QDialog d;
    QTextEdit info;
    info.setReadOnly(true);
    QString s;
    //s=m_parser->dataset().info();
    info.setText(s);
    d.setLayout(new QVBoxLayout);
    d.layout()->addWidget(&info);
    d.exec();
}

void vpParserWidget::on_btn_delete_clicked()
{
    if(QMessageBox::Yes == QMessageBox::question(this, "Danger,Will robinson!", "Are you fucking sure?", QMessageBox::Yes|QMessageBox::No))
    {
        m_parser->deleteLater();
    }
}

void vpParserWidget::on_btn_record_clicked(bool activated)
{
    if(activated)
    {
        QString filter="CSV("+m_parser->source()+".csv)";
        QString filename=QFileDialog::getSaveFileName(this,"Save file",QString(),filter);
        if(!filename.isEmpty())
        {
            if(!filename.endsWith(".csv"))filename.append(".csv");
            m_parser->recordCSV(filename);
        }
        else
        {
            ui->btn_record->setChecked(false);
        }
    }
    else
    {
        if(QMessageBox::Yes == QMessageBox::question(this, "Sure?", "Stop recording?", QMessageBox::Yes|QMessageBox::No))
        {
            m_parser->stopRecord();
        }
        else
        {
            ui->btn_record->setChecked(true);
        }
    }
}
