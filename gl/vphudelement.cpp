#include "vphudelement.h"

vpHudElement::vpHudElement(uint c_x,uint c_y, uint w, uint h,QColor color) :
    QObject()
{
    m_center = QVector2D(c_x,c_y);
    m_size = QVector2D(w,h);
    m_type = HudUnknow;
    m_color = color;
    m_draw_border = true;
    m_relative_calculated = false;
}

vpHudElement::vpHudElement(QVector2D rel_center, QVector2D rel_size, QColor color) :
    QObject()
{
    m_rel_center = rel_center;
    m_rel_size = rel_size;
    m_type = HudUnknow;
    m_color = color;
    m_draw_border = true;
    m_relative_calculated = true;
}


void vpHudElement::draw(QPainter &p)
{
    if(!m_relative_calculated)
    {
        m_rel_center = QVector2D(m_center.x()/p.window().width(),
                                 m_center.y()/p.window().height());
        m_rel_size = QVector2D(m_size.x()/p.window().width(),
                               m_size.y()/p.window().height());
        m_relative_calculated =true;
    }
    else
    {
        m_center = QVector2D(m_rel_center.x()*p.window().width(),
                                 m_rel_center.y()*p.window().height());
        m_size = QVector2D(m_rel_size.x()*p.window().width(),
                               m_rel_size.y()*p.window().height());
    }
    if (m_draw_border)
    {
        p.setPen(Qt::blue);
        p.setBrush(QColor(0,0,0,0));
        p.drawRect(rect());
    }
    draw_specific(p);
}

void vpHudElement::mouse_in(QMouseEvent *event)
{
    if (rect().contains(event->pos()))
    {
        if(event->buttons())    click(event);
        else                    hover(event);

        event->accept();
    }
}

QRect vpHudElement::rect()
{ 
    return QRect( m_center.x() - (m_size.x()/2),
                  m_center.y() - (m_size.y()/2),
                  m_size.x(),
                  m_size.y() );
}

void vpHudElement::hover(QMouseEvent *event)
{
    //qDebug() << "[INFO]vpHudElement::hover->Hovered!" << event->pos() ;
}

void vpHudElement::click(QMouseEvent *event)
{
    start_alpha_animation();
}

void vpHudElement::start_alpha_animation()
{
    m_color.setAlpha(m_color.alpha()+100);
    m_alpha_animation_step = 0;
    m_timer.start(50);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(alpha_animation_step()));
    emit updated();
}

void vpHudElement::alpha_animation_step()
{
    if (m_alpha_animation_step < 10)
    {
        m_color.setAlpha(m_color.alpha()-10);
        m_alpha_animation_step++;
        emit updated();
    }
    else
    {
        m_timer.stop();
        disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(alpha_animation_step()));
    }
}
