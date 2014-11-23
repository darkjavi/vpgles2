#include "vpdatasourcewidget.h"
#include "ui_vpdatasourcewidget.h"
#include <QFileDialog>
#include <QMenu>

vpDataSourceWidget::vpDataSourceWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::vpDataSourceWidget)
{
    ui->setupUi(this);
}

vpDataSourceWidget::~vpDataSourceWidget()
{
    delete ui;
}

void vpDataSourceWidget::setDataSource(vpDataSource *ds)
{
    m_dataSource = ds;
}

void vpDataSourceWidget::on_btn_exportCSV_clicked()
{
    QString filter="CSV(*.csv)";
    QString filename=QFileDialog::getSaveFileName(this,"Save file",QString(),filter);
    if(filename.isEmpty())return;
    if(!filename.endsWith(".csv"))filename.append(".csv");
    QMetaObject::invokeMethod(m_dataSource,"exportCSV",Qt::QueuedConnection,Q_ARG(QString,filename));
}

void vpDataSourceWidget::on_btn_open_clicked()
{
    ui->btn_open->setChecked(true);
    QMenu main;
    QMenu files("Load Files");
    QMenu devices("Connect Device");
    QMenu bd("Load DB");
    QAction* gcsv = files.addAction("Generic CSV");
    QAction* tcsv = files.addAction("Torque CSV");
    QAction* nmea = files.addAction("NMEA");
    QAction* btmf = devices.addAction("bt-mindflex");
    QAction* gpsd = devices.addAction("GPSD");

    main.addMenu(&files);
    main.addMenu(&devices);
    main.addMenu(&bd);
    main.setFont(QFont("ubuntu",8));
    files.setFont(QFont("ubuntu",8));
    devices.setFont(QFont("ubuntu",8));
    QAction* selected = main.exec(this->mapToGlobal(this->pos()));
    if(selected == gcsv)
    {
        QFileDialog f;
        f.exec();
        if(f.selectedFiles().count())
        {
            m_dataSource->importCSV(f.selectedFiles().at(0));
        }
    }else if(selected == tcsv){

    }else if(selected == nmea){

    }else if(selected == btmf){

    }else if(selected == gpsd){

    }
    ui->btn_open->setChecked(false);
}
