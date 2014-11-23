#include "vp3dgeoaxis.h"

vp3DGeoAxis::vp3DGeoAxis(QVector3D pos,QVector3D size, qreal div_size) : vp3DGeoObj(pos,size)
{
    m_type      = vp3DGeoObj::TypeAxis;
    m_id        = "vp3DGeoAxis";
    m_div_size = div_size;
    generate_geometry();
}

QVector3D vp3DGeoAxis::get_center()
{
    return m_pos;
}

void vp3DGeoAxis::generate_geometry()
{
    m_components.clear();
    generate_axis();
    generate_planes();
}

void vp3DGeoAxis::generate_axis()
{
    vp3DGeoPart y_axis;
        y_axis.draw_mode() = GL_LINES;
        y_axis.line_size() = 2;
        y_axis.color().setRgb(0, 255, 0);
        y_axis.vertex_array() << QVector3D(0,  (m_size.y()/2),0);
        y_axis.vertex_array() << QVector3D(0, -(m_size.y()/2),0);
        m_components.append(y_axis);

    vp3DGeoPart x_axis;
        x_axis.draw_mode() = GL_LINES;
        x_axis.line_size() = 2;
        x_axis.color().setRgb(255, 0, 0);
        x_axis.vertex_array() << QVector3D(  (m_size.x()/2), 0, 0);
        x_axis.vertex_array() << QVector3D( -(m_size.x()/2), 0, 0);
        m_components.append(x_axis);

    vp3DGeoPart z_axis;
        z_axis.draw_mode() = GL_LINES;
        z_axis.line_size() = 2;
        z_axis.color().setRgb(0, 0, 255);
        z_axis.vertex_array() << QVector3D(0, 0,  (m_size.z()/2));
        z_axis.vertex_array() << QVector3D(0, 0, -(m_size.z()/2));
        m_components.append(z_axis);
}

void vp3DGeoAxis::generate_planes()
{
    //Y = 0
    vp3DGeoPart y_plane;
        y_plane.draw_mode() = GL_LINES;
        y_plane.color().setRgb(150, 127, 127);
        for(int div = 0 ; div <= (m_size.x()/m_div_size) ; div++ )
        {
            qreal x_pos = -(m_size.x()/2) + (m_div_size*div);
            y_plane.vertex_array() << QVector3D( x_pos ,0,(m_size.z()/2));
            y_plane.vertex_array() << QVector3D( x_pos ,0,-(m_size.z()/2));
        }
        for(int div = 0 ; div <= (m_size.z()/m_div_size) ; div++ )
        {
            qreal z_pos = -(m_size.z()/2) + (m_div_size*div);
            y_plane.vertex_array() << QVector3D( (m_size.x()/2) ,0, z_pos);
            y_plane.vertex_array() << QVector3D( -(m_size.x()/2) ,0, z_pos);
        }
        m_components.append(y_plane);

    //X = 0
    vp3DGeoPart x_plane;
        x_plane.draw_mode() = GL_LINES;
        x_plane.color().setRgb(127, 127, 150);
        for(int div = 0 ; div <= (m_size.y()/m_div_size) ; div++ )
        {
            qreal y_pos = -(m_size.y()/2) + (m_div_size*div);
            x_plane.vertex_array() << QVector3D( 0, y_pos,  (m_size.z()/2));
            x_plane.vertex_array() << QVector3D( 0, y_pos, -(m_size.z()/2));
        }
        for(int div = 0 ; div <= (m_size.z()/m_div_size) ; div++ )
        {
            qreal z_pos = -(m_size.z()/2) + (m_div_size*div);
            x_plane.vertex_array() << QVector3D( 0,  (m_size.y()/2), z_pos);
            x_plane.vertex_array() << QVector3D( 0, -(m_size.y()/2), z_pos);
        }
        m_components.append(x_plane);

    //Z = 0
    vp3DGeoPart z_plane;
        z_plane.draw_mode() = GL_LINES;
        z_plane.color().setRgb(127, 150, 127);
        for(int div = 0 ; div <= (m_size.x()/m_div_size) ; div++ )
        {
            qreal x_pos = -(m_size.x()/2) + (m_div_size*div);
            z_plane.vertex_array() << QVector3D( x_pos,  (m_size.y()/2), 0);
            z_plane.vertex_array() << QVector3D( x_pos, -(m_size.y()/2), 0);
        }
        for(int div = 0 ; div <= (m_size.y()/m_div_size) ; div++ )
        {
            qreal y_pos = -(m_size.y()/2) + (m_div_size*div);
            z_plane.vertex_array() << QVector3D(  (m_size.x()/2), y_pos, 0);
            z_plane.vertex_array() << QVector3D( -(m_size.x()/2), y_pos, 0);
        }
        m_components.append(z_plane);
}

