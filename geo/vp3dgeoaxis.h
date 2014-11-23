#ifndef VP3DGEOAXIS_H
#define VP3DGEOAXIS_H

#include "vp3dgeoobj.h"

class vp3DGeoAxis : public vp3DGeoObj
{
public:
    vp3DGeoAxis(QVector3D pos = QVector3D(0,0,0),QVector3D size = QVector3D(100,100,100), qreal div_size = 50);
protected:
    qreal m_div_size;
    QVector3D get_center();
    void generate_geometry();
    void generate_axis();
    void generate_planes();
};

#endif // VP3DGEOAXIS_H
