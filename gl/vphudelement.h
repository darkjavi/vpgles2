#ifndef VPHUDELEMENT_H
#define VPHUDELEMENT_H

#include <QObject>
#include <QVector2D>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>

class vpHudElement : public QObject
{
    Q_OBJECT
public:
    enum ElementType
    {
        HudUnknow,
        HudButtonRect,
        HudScrollBar
    };

    explicit vpHudElement(uint c_x,uint c_y, uint w, uint h,QColor color=QColor(255,0,0,255));
    vpHudElement(QVector2D rel_center, QVector2D rel_size, QColor color=QColor(255,0,0,255));

    void  draw(QPainter &p);
    QRect rect();
protected:
    QVector2D   m_center;
    QVector2D   m_size;
    QVector2D   m_rel_center;
    QVector2D   m_rel_size;
    bool        m_relative_calculated;

    quint8    m_type;
    QColor    m_color;
    bool      m_draw_border;

    QTimer   m_timer;

    quint8 m_alpha_animation_step;
    void start_alpha_animation();
    virtual void draw_specific(QPainter &p)=0;
    virtual void click(QMouseEvent *event);
    virtual void hover(QMouseEvent *event);

signals:
    void updated();
private slots:
    void alpha_animation_step();
public slots:
    void mouse_in(QMouseEvent *event);
};

#endif // VPHUDELEMENT_H
