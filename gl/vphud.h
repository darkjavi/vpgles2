#ifndef VPHUD_H
#define VPHUD_H

#include <QObject>
#include "vphudincludes.h"
#include <QMouseEvent>

class vpHud : public QObject
{
    Q_OBJECT
public:
    explicit vpHud(QObject *parent = 0);
    void draw(QPainter &p);
    void clear_hud();
private:
    QVector<vpHudElement*> m_elements;

    void append_element(vpHudElement* hud_element);
signals:
    void mouse_out(QMouseEvent* event);
    void updated();
public slots:
    void mouse_in(QMouseEvent* event);
};

#endif // VPHUD_H
