#include "vp3dgeoprimitive.h"

vp3DGeoPrimitive::vp3DGeoPrimitive(quint8 primitive_type, QVector3D pos, QVector3D size, QColor color, ushort complexity)
    :vp3DGeoObj(pos,size,color)
{
    m_type  = vp3DGeoObj::TypePrimitive;
    if(primitive_type == PrimitiveCube)
    {
        m_id = "vp3DGeoPrimitive-Cube";
    }else if (primitive_type == PrimitiveSphere)
    {
        m_id = "vp3DGeoPrimitive-Sphere";
    }else if (primitive_type == PrimitiveTorus)
    {
        m_id = "vp3DGeoPrimitive-Torus";
    }else
    {
        qDebug() << "[ERROR]vp3DGeoPrimitive::vp3DGeoPrimitive->Primitiva desconocida:" <<primitive_type;
    }
    m_primitive_type = primitive_type;
    m_complexity  = complexity;

    m_draw_points = true;
    m_draw_lines  = true;
    //m_draw_faces  = false;
    generate_geometry();
}

QVector3D vp3DGeoPrimitive::get_center()
{
    return m_pos;//mmmm?
}

void vp3DGeoPrimitive::generate_geometry()
{
    m_components.clear();
    if(m_primitive_type == PrimitiveCube)
    {

    }else if (m_primitive_type == PrimitiveSphere)
    {
        m_components.append(generate_sphere(m_pos,m_size,m_color,m_complexity));
    }else if (m_primitive_type == PrimitiveTorus)
    {

    }else
    {
        qDebug() << "[ERROR]vp3DGeoPrimitive::generate_geometry()->Primitiva desconocida:" <<m_primitive_type;
    }
}

vp3DGeoPart vp3DGeoPrimitive::generate_sphere(QVector3D pos, QVector3D size, QColor color,ushort complexity)
{
    vp3DGeoPart n_part;
    n_part.draw_mode()  = GL_TRIANGLES;
    n_part.color()      = color;
    qreal radius = size.x();
    ushort slices = complexity;
    slices >>= 1;//Eliminamos el ultimo bit y lo volvemos a shiftear para dejarlo en numero par
    slices <<= 1;
    qreal subSlice=2*M_PI/slices;
    for (ushort i = 0 ; i < slices ; i++)
    {
        qreal cos_Is = cos(i*subSlice),cos_I1s = cos((i+1)*subSlice);
        qreal sin_Is = sin(i*subSlice),sin_I1s = sin((i+1)*subSlice);
        for ( short j = -(slices/2) ; j < (slices/2) ; j++ )
        {
            qreal cos_Js = cos(j*subSlice), cos_J1s = cos((j+1)*subSlice);
            qreal sin_Js = sin(j*subSlice), sin_J1s = sin((j+1)*subSlice);

            QVector3D vertex0;
            vertex0.setX(radius*cos_Is*sin_Js);
            vertex0.setY(radius*sin_Is*sin_Js);
            vertex0.setZ(radius*cos_Js);
            QVector3D vertex1;
            vertex1.setX(radius*cos_I1s*sin_Js);
            vertex1.setY(radius*sin_I1s*sin_Js);
            vertex1.setZ(radius*cos_Js);
            QVector3D vertex2;
            vertex2.setX(radius*cos_I1s*sin_J1s);
            vertex2.setY(radius*sin_I1s*sin_J1s);
            vertex2.setZ(radius*cos_J1s);
            n_part.vertex_array() << vertex0 << vertex1 << vertex2;
        }
    }
    return n_part;
}
