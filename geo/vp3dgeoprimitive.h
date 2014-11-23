#ifndef VP3DGEOPRIMITIVE_H
#define VP3DGEOPRIMITIVE_H

#include "vp3dgeoobj.h"

class vp3DGeoPrimitive : public vp3DGeoObj
{
public:
enum PrimitiveType
{
    PrimitiveCube,
    PrimitiveSphere,
    PrimitiveTorus
};


vp3DGeoPrimitive(quint8 primitive_type,  QVector3D pos,QVector3D size, QColor color = QColor(250,0,0) ,ushort complexity = 8);
protected:
    quint8 m_primitive_type;
    ushort m_complexity;
    QVector3D get_center();
    void generate_geometry();
    static vp3DGeoPart generate_sphere(QVector3D pos,QVector3D size, QColor color = QColor(250,0,0), ushort complexity = 8);
};

#endif // VP3DGEOPRIMITIVE_H
