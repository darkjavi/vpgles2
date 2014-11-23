#include "vphudscrollbar.h"

vpHudScrollBar::vpHudScrollBar(QVector2D rel_center, QVector2D rel_size, quint8 bar_type, QColor color) :
    vpHudElement(rel_center,rel_size,color)
{
    m_type = vpHudElement::HudScrollBar;
    m_bar_type = bar_type;
}
vpHudScrollBar::vpHudScrollBar(uint c_x, uint c_y, uint w, uint h, quint8 bar_type, QColor color) :
    vpHudElement(c_x,c_y,w,h,color)
{
    m_type = vpHudElement::HudScrollBar;
    m_bar_type = bar_type;
}

void vpHudScrollBar::draw_specific(QPainter &p)
{

}
