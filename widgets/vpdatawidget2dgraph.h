#ifndef VPDATAWIDGET2DGRAPH_H
#define VPDATAWIDGET2DGRAPH_H
#include "vpdatawidget.h"
#include <QGraphicsView>

class vpDataWidget2DGraph : public vpDataWidget
{
    Q_OBJECT
public:
    explicit vpDataWidget2DGraph(vpDataPool* datapool, QWidget *parent = 0);
protected:
    QGraphicsView m_graph;
    QVector<QGraphicsItem*> m_axisComponents;
    void drawAxis();
    void drawGraphLines();
signals:
    
public slots:
    void refresh();
};

#endif // VPDATAWIDGET2DGRAPH_H
