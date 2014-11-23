#ifndef VP3DGEOPART_H
#define VP3DGEOPART_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QtOpenGL>

class vp3DGeoPart
{
public:
    vp3DGeoPart();

    QVector<QVector3D>&  vertex_array()     {return m_vertex_array;}
    QVector<QVector2D>&  text_coords()      {return m_text_coords;}
    QVector<QVector2D>&  normal()           {return m_normal;}
    QColor            &  color()            {return m_color;}
    quint16           &  texture_handle()   {return m_texture_handle;}
    QString           &  texture_id()       {return m_texture_id;}
    quint8            &  draw_mode()        {return m_draw_mode;}
    quint8            &  line_size()        {return m_line_size;}
private:
    QVector<QVector3D>  m_vertex_array;
    QVector<QVector2D>  m_text_coords;
    QVector<QVector2D>  m_normal;
    QColor              m_color;
    quint16             m_texture_handle;
    QString             m_texture_id;
    quint8              m_draw_mode;
    quint8              m_line_size;
};

#endif // VP3DGEOPART_H
