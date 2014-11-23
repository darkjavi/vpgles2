#include "vp3dgeocamera.h"

vp3DGeoCamera::vp3DGeoCamera(vpTCamera *camera) : vp3DGeoObj(camera->get_cam_pos(),QVector3D(0,0,0) )
{
    //m_type      = vp3DGeoObj::TypeAxis;
    m_id        = "vp3DGeoCamera";
    m_draw_fov = false;
    m_camera = camera;
    generate_geometry();
}

QVector3D vp3DGeoCamera::get_center()
{
    return m_pos;
}

void vp3DGeoCamera::generate_geometry()
{

}

ulong vp3DGeoCamera::draw2D(QPainter *p)
{
    qreal h_bar_proportion = 0.1;
    qreal v_bar_proportion = 0.1;

    int h_start  = p->viewport().width()  * (1-h_bar_proportion);
    int h_length = p->viewport().width()  * (h_bar_proportion);
    int v_start  = p->viewport().height() * (1-v_bar_proportion);
    int v_length = p->viewport().height() * (v_bar_proportion);

    p->setBrush(QColor(255,0,0,10));//el brush es para el color de fondo y el pen para los bordes
    p->setPen(QColor(255,0,0,50));//el brush es para el color de fondo y el pen para los bordes

    QRect h_scale(QPoint(0,v_start),QSize(p->viewport().width(),v_length));
    p->drawRect(h_scale);

    QRect v_scale(QPoint(h_start,0),QSize(h_length,p->viewport().height()));
    p->drawRect(v_scale);

    p->setBrush(QColor(255,0,0,50));//el brush es para el color de fondo y el pen para los bordes
    p->setPen(QColor(255,0,0,100));//el brush es para el color de fondo y el pen para los bordes

    int h_pos = 0;
    int h_bar_size = p->viewport().width()*0.1;
    QRect h_bar(h_pos,v_start,h_bar_size,v_length);
    p->drawRoundedRect(h_bar,5,5);

    int v_pos = 0;
    int v_bar_size = p->viewport().height() * 0.1;
    QRect v_bar(h_start,v_pos,h_length,v_bar_size);
    p->drawRoundedRect(v_bar,5,5);
    return 0;
}

void vp3DGeoCamera::update()
{
    generate_geometry();
}
