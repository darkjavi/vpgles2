#include "vp3dgeodataobj.h"

vp3DGeoDataObj::vp3DGeoDataObj(QVector3D pos, QVector3D size, vpDataSet *dataset)
    : vp3DGeoObj(pos,size)
{
    if ( dataset )
    {
        set_dataset(dataset);
    }
}

void vp3DGeoDataObj::set_dataset(vpDataSet *dataset)
{
    clear_data();
    m_dataset = dataset;
    connect(m_dataset,SIGNAL(data_updated()),this,SLOT(update()));
    connect(m_dataset,SIGNAL(destroyed()),this,SLOT(clear_data()));
    update();
}

void vp3DGeoDataObj::set_selectedFields(QVector<ushort> selected_indexs)
{
    m_selected_fields = selected_indexs;
    update();
}

void vp3DGeoDataObj::clear_data()
{
    if ( m_dataset )
    {
        disconnect(m_dataset,SIGNAL(data_updated()),this,SLOT(update()));
        disconnect(m_dataset,SIGNAL(destroyed()),this,SLOT(clear_data()));
        m_selected_fields.clear();
        m_dataset = 0;
    }
    update();
}

void vp3DGeoDataObj::update()
{
    generate_geometry();
}
