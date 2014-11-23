#include "vphud.h"

vpHud::vpHud(QObject *parent) :
    QObject(parent)
{
    for ( int i = 0 ; i < 8 ; i++)
    {
        QVector2D pos(0.9-(i/10.0f),0.1);
        QVector2D size(0.05,0.1);
        vpHudButtonRect* n_btn = new vpHudButtonRect(pos,size);
        append_element(n_btn);
    }
    QVector2D pos(0.5,1.0);
    QVector2D size(1.0,0.1);
    vpHudScrollBar* h_scroll = new vpHudScrollBar(pos,size,vpHudScrollBar::ScrollHorizontal);
    append_element(h_scroll);
}

void vpHud::draw(QPainter &p)
{
    for ( int i = 0 ; i < m_elements.count() ; i++ )
    {
        m_elements[i]->draw(p);
    }
}

void vpHud::clear_hud()
{
    for (int i = 0 ; i < m_elements.count() ; i ++)
    {
        delete m_elements[i];
        //Hacer con el destructor especifico!!!
    }
}

void vpHud::append_element(vpHudElement *hud_element)
{
    m_elements.append(hud_element);
    connect(hud_element,SIGNAL(updated()),this,SIGNAL(updated()));
}



void vpHud::mouse_in(QMouseEvent* event)
{
    for (int i = 0 ; i < m_elements.count() ; i ++)
    {
        m_elements[i]->mouse_in(event);
    }
    if( ! event->isAccepted() )
    {
        emit mouse_out(event);
    }
}
