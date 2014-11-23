#ifndef VPHUDSCROLLBAR_H
#define VPHUDSCROLLBAR_H
#include "vphudelement.h"

class vpHudScrollBar : public vpHudElement
{
    Q_OBJECT
public:
    enum BarType
    {
        ScrollHorizontal,
        ScrollVertical
    };

    explicit vpHudScrollBar(uint c_x,uint c_y, uint w, uint h, quint8 bar_type = vpHudScrollBar::ScrollHorizontal, QColor color=QColor(255,0,0,255));
             vpHudScrollBar(QVector2D rel_center, QVector2D rel_size, quint8 bar_type = vpHudScrollBar::ScrollHorizontal, QColor color=QColor(255,0,0,255));
protected:
    void draw_specific(QPainter &p);
    quint8 m_bar_type;
signals:
    
public slots:
    
};

#endif // VPHUDSCROLLBAR_H
