#include "vpdatawidget.h"
#include "ui_vpdatawidget.h"
#include <QMouseEvent>

vpDataWidget::vpDataWidget(vpDataPool *datapool, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::vpDataWidget)
{
    ui->setupUi(this);
    m_dataPool=datapool;
    m_autoSelection=true;
    m_selectedDatasets << datapool->dataSets();//auto seleccionamos los datasets que ya estan en  la piscina
    populateUI();
    connect(m_dataPool,SIGNAL(updated()),this,SLOT(dataPoolUpdated()));
    connect(m_dataPool,SIGNAL(destroyed()),this,SLOT(dataPoolDeleted()));
    connect(m_dataPool,SIGNAL(newDataSet(vpDataSet*)),this,SLOT(newDataset(vpDataSet*)));
    this->setWindowTitle("DataWidget");
    this->setMouseTracking(true);


    //desplazar el splitter (funciona?)
    //QList<int> currentSizes = ui->splitter_view->sizes();
    //int total = currentSizes[0]+currentSizes[1];
    //QList<int> newSizes;
    //newSizes.append(total*0.7);
    //newSizes.append(total-newSizes.last());
    //ui->splitter_view->setSizes(newSizes);
}

vpDataWidget::~vpDataWidget()
{
    delete ui;
}

void vpDataWidget::addView(QWidget *dataview)
{
    ui->layout_view->addWidget(dataview);
}

void vpDataWidget::populateUI()
{
    ui->combo_xField->blockSignals(true);
    ui->combo_zField->blockSignals(true);
    ui->list_yFields->blockSignals(true);
    ui->listDataSets->blockSignals(true);
    ui->listDataSets->clear();
    ui->list_yFields->clear();
    ui->combo_xField->clear();
    ui->combo_zField->clear();
    if(m_dataPool->count() >0)
    {
        qDebug()<<m_dataPool->count();
        for(int i = 0 ; i < m_dataPool->count() ; i++)
        {
            vpDataSet* dataset = m_dataPool->dataSets()[i];
            QListWidgetItem* item = new QListWidgetItem(dataset->getId());
            if(m_selectedDatasets.contains(dataset))    item->setCheckState(Qt::Checked);
            else                                        item->setCheckState(Qt::Unchecked);
            ui->listDataSets->addItem(item);
        }
        if(!m_selectedDatasets.isEmpty())
        {
            if(!m_dataPool->dataSets().contains(m_selectedDatasets[0]))//comprobamos que el dataset seleccionado sigue existiendo
            {
                qDebug()<<"[WARN]Selected DataSet is not on the DataPool!";
            }
            else
            {
                QVector<QString> fields = m_selectedDatasets[0]->getFields();
                if(m_autoSelection)m_selectedYFields.clear();
                for(int i = 0;i< fields.count();i++)
                {
                    QListWidgetItem* item=new QListWidgetItem(fields[i]);
                    if(m_autoSelection)
                    {
                        item->setCheckState(Qt::Checked);
                        m_selectedYFields.append(fields[i]);
                    }
                    else
                    {
                        if(m_selectedYFields.contains(fields[i]))   item->setCheckState(Qt::Checked);
                        else                                        item->setCheckState(Qt::Unchecked);
                    }
                    ui->list_yFields->addItem(item);
                    ui->combo_xField->addItem(fields[i]);
                    ui->combo_zField->addItem(fields[i]);
                }
            ui->combo_xField->setCurrentIndex(fields.indexOf(m_selectedXField));
            ui->combo_zField->setCurrentIndex(fields.indexOf(m_selectedZField));
            }
        }
    }
    ui->combo_xField->blockSignals(false);
    ui->combo_zField->blockSignals(false);
    ui->list_yFields->blockSignals(false);
    ui->listDataSets->blockSignals(false);
}

void vpDataWidget::dataPoolUpdated()
{
    populateUI();
}

void vpDataWidget::dataSetUpdated()
{
    QMetaObject::invokeMethod(this,"refresh");
}

void vpDataWidget::dataPoolDeleted()
{
    this->deleteLater();
}

void vpDataWidget::dataSetDeleted(QObject *obj)
{
    qDebug()<<"[DEBUG]vpDataWidget::dataSetDeleted->"<<obj;
    removeDataSet((vpDataSet*) obj);
}

void vpDataWidget::newDataset(vpDataSet *ds)
{
    if(m_autoSelection)
        addDataSet(ds);
}

void vpDataWidget::addDataSet(vpDataSet* ds)
{
    if(!m_selectedDatasets.contains(ds))
    {
        qDebug()<<"[DEBUG]vpDataWidget::addDataSet->"<<ds;
        m_selectedDatasets.append(ds);
        connect(ds,SIGNAL(destroyed(QObject*)),this,SLOT(dataSetDeleted(QObject*)));
        connect(ds,SIGNAL(dataUpdated()),this,SLOT(dataSetUpdated()));

        populateUI();
        QMetaObject::invokeMethod(this,"refresh");
    }
}

void vpDataWidget::removeDataSet(vpDataSet* ds)
{
    qDebug()<<"[DEBUG]vpDataWidget::removeDataSet->looking:"<<ds;
    if(m_selectedDatasets.contains(ds))
    {
        qDebug()<<"[DEBUG]vpDataWidget::removeDataSet->removing:"<<ds;
        m_selectedDatasets.remove(m_selectedDatasets.indexOf(ds));
        disconnect(ds,SIGNAL(dataUpdated()),this,SLOT(dataSetUpdated()));
        disconnect(ds,SIGNAL(destroyed(QObject*)),this,SLOT(dataSetDeleted(QObject*)));

        populateUI();
        QMetaObject::invokeMethod(this,"refresh");
    }
}

void vpDataWidget::clear()
{

}

void vpDataWidget::on_combo_xField_currentIndexChanged(QString field)
{
    if(field.isEmpty()) return;
    m_selectedXField = field;
    qDebug()<<"x field:"<<field;
    QMetaObject::invokeMethod(this,"refresh");
}

void vpDataWidget::on_combo_zField_currentIndexChanged(QString field)
{
    if(field.isEmpty()) return;
    m_selectedZField = field;
    qDebug()<<"z field:"<<field;
    QMetaObject::invokeMethod(this,"refresh");
}

void vpDataWidget::on_list_yFields_itemChanged(QListWidgetItem* item)
{
    if(item->checkState()==Qt::Checked)
    {
        if(!m_selectedYFields.contains(item->text()))
        {
            m_selectedYFields.append(item->text());
            QMetaObject::invokeMethod(this,"refresh");
        }
    }
    else
    {
        if(m_selectedYFields.contains(item->text()))
        {
            m_selectedYFields.remove(m_selectedYFields.indexOf(item->text()));
            QMetaObject::invokeMethod(this,"refresh");
        }
    }
}

void vpDataWidget::on_listDataSets_itemChanged(QListWidgetItem* item)
{
    if(item->checkState()==Qt::Checked)
    {
        for(int i = 0 ; i<m_dataPool->dataSets().count();i++)
        {
            if(m_dataPool->dataSets()[i]->getId()==item->text())
            {
                addDataSet(m_dataPool->dataSets()[i]);
                break;
            }
        }
    }
    else
    {
        for(int i = 0 ; i<m_dataPool->dataSets().count();i++)
        {
            if(m_dataPool->dataSets()[i]->getId()==item->text())
            {
                removeDataSet(m_dataPool->dataSets()[i]);
                break;
            }
        }
    }
}

void vpDataWidget::on_btn_reset_clicked()
{
    //QList<int> currentSizes = ui->splitter_view->sizes();
    //qDebug()<< currentSizes;
    //int total = currentSizes[0]+currentSizes[1];
    //QList<int> newSizes;
    //newSizes.append(total*0.7);
    //newSizes.append(total-newSizes.last());
    //ui->splitter_view->setSizes(newSizes<<1000<<10);
    //ui->tabWidget->hide();
    ui->splitter->setSizes(QList<int>() << ui->splitter->sizes()[0]+100 << ui->splitter->sizes()[1]-100);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(animateClose()));
    m_timer.start(50);
}

void vpDataWidget::animateClose()
{
    qDebug() <<  ui->splitter->sizes();
    if(ui->splitter->sizes()[0] > 0)
    {
        ui->splitter->setSizes(QList<int>() << (ui->splitter->sizes()[0]-10) << ui->splitter->sizes()[1]+10);
        qDebug()<< (ui->splitter->sizes()[0]-10) << ui->splitter->sizes()[1]+10;
    }
    else
    {
        m_timer.stop();
        disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(animateClose()));
    }
}

void vpDataWidget::animateOpen()
{
    if(ui->splitter->sizes()[0] < 50)
    {
        ui->splitter->setSizes(QList<int>() << ui->splitter->sizes()[0]+10 << ui->splitter->sizes()[1]);
    }
    else
    {
        m_timer.stop();
        disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(animateOpen()));
    }
}

//manejador del raton
void vpDataWidget::mouseMoveEvent(QMouseEvent *e)
{
    qDebug()<<e;
    qDebug()<<e->x()<<e->y();
}
