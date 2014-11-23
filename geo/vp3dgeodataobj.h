#ifndef VP3DGEODATAOBJ_H
#define VP3DGEODATAOBJ_H

#include "vp3dgeoobj.h"
#include "vpdatapool.h"

class vp3DGeoDataObj : public vp3DGeoObj
{
    Q_OBJECT
public:
    vp3DGeoDataObj(QVector3D pos, QVector3D size, vpDataSet* dataset = 0);

protected:
    vpDataSet*          m_dataset;
    QVector<ushort>     m_selected_fields;
signals:

public slots:
    void set_dataset(vpDataSet* dataset);
    void set_selectedFields(QVector<ushort> selected_indexs);
    void clear_data();
    void update();
};

#endif // VP3DGEODATAOBJ_H
