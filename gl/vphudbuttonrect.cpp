#include "vphudbuttonrect.h"

vpHudButtonRect::vpHudButtonRect(uint c_x, uint c_y, uint w, uint h, QColor color) :
    vpHudElement(c_x,c_y,w,h)
{
    m_type = vpHudElement::HudButtonRect;
}

vpHudButtonRect::vpHudButtonRect(QVector2D rel_center, QVector2D rel_size, QColor color) :
    vpHudElement(rel_center,rel_size,color)
{
    m_type = vpHudElement::HudButtonRect;
}

void vpHudButtonRect::draw_specific(QPainter &p)
{
    p.setBrush(m_color);
    p.drawRect(rect());
}
