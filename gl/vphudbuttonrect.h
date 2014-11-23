#ifndef VPHUDBUTTONRECT_H
#define VPHUDBUTTONRECT_H

#include "vphudelement.h"

class vpHudButtonRect : public vpHudElement
{
    Q_OBJECT
public:
    explicit vpHudButtonRect(uint c_x,uint c_y, uint w, uint h,QColor color=QColor(255,0,0,255));
             vpHudButtonRect(QVector2D rel_center, QVector2D rel_size, QColor color=QColor(255,0,0,255));
protected:
    void draw_specific(QPainter &p);
signals:
    
public slots:
    
};

#endif // VPHUDBUTTONRECT_H
