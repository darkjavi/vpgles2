#include "vpwidgetdata.h"
vpWidgetData::vpWidgetData(vpDataPool *dataPool,QWidget *parent) :
    QFrame(parent)
{
    m_dataPool    = 0;
    m_dataSet     = 0;
    set_dataPool(dataPool);
    m_shared_dataPool = true;
    connect(&m_dataPoolComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxHelper(int)));
}

vpWidgetData::vpWidgetData(QWidget *parent) :
    QFrame(parent)
{
    m_dataPool    = 0;
    m_dataSet     = 0;
    vpDataPool* n_dataPool = new vpDataPool;
    set_dataPool(n_dataPool);
    m_shared_dataPool = false;
    connect(&m_dataPoolComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxHelper(int)));
}

vpWidgetData::~vpWidgetData()
{
    if ( ! m_shared_dataPool )
    {
        delete m_dataPool;
    }
}

//SLOTS

void vpWidgetData::comboBoxHelper(int index)
{
    if(index < 0) return;
    if(m_dataPool->dataSets().count() >= index)
    {
        set_dataSet(m_dataPool->dataSets()[index]);
    }
}

void vpWidgetData::set_dataSet(vpDataSet *dataset)
{
    clear_data();
    m_dataSet = dataset;
    connect(m_dataSet,SIGNAL(destroyed()),this,SLOT(clear()));
    connect(m_dataSet,SIGNAL(data_updated()),this,SLOT(dataSet_updated()));
    //llamada a virtual puro!!!! Ojo con esto
    dataSet_updated();
    //emit new_dataSet(dataset);
}

void vpWidgetData::set_dataPool(vpDataPool *dataPool)
{
    clear();
    m_dataPool = dataPool;
    connect(m_dataPool,SIGNAL(updated()),this,SLOT(dataPool_updated()));
    connect(m_dataPool,SIGNAL(destroyed()),this,SLOT(clear()));
    dataPool_updated();
    emit new_dataPool(dataPool);
}

void vpWidgetData::dataPool_updated()
{
    if(m_dataPool)
    {
        m_dataPoolComboBox.clear();
        if(m_dataPool->dataSets().count() == 0) m_dataPoolComboBox.addItem("NONE");
        for(int i = 0 ; i < m_dataPool->dataSets().count() ; i++)
        {
            m_dataPoolComboBox.addItem(m_dataPool->dataSets()[i]->id());
        }
        if ( (m_dataPool->dataSets().count() > 0) && (!m_dataSet) )
        {//si no hay nada seleccionado selecciona por defecto el primer datset
            set_dataSet(m_dataPool->dataSets()[0]);
        }
        m_dataPoolComboBox.setEnabled(true);
    }
    else
    {
        m_dataPoolComboBox.clear();
        m_dataPoolComboBox.setEnabled(false);
    }

//    else if (m_datasource->data_pool().count() == 0)
//    {
        //dataset_updated();//si el datasource esta vacio forzamos una actualizacion;
        //por alguna razon esto peta alegando llamada a metodo virtual WTF!
//    }
    //refrescar los menus que muestran los datasets
//creo que ya no hace falta
}

void vpWidgetData::clear()
{
    disconnect(this,SLOT(dataSet_updated()));
    disconnect(this,SLOT(dataPool_updated()));
    disconnect(this,SLOT(clear()));

    if ( (! m_shared_dataPool) && (m_dataPool) )
    {
        delete m_dataPool;
        m_shared_dataPool = false;
    }
    m_dataPool = 0;
    m_dataSet  = 0;
    clear_data();
    dataPool_updated();
}
