#include "vpdatawidget2dgraph.h"
#include <QGraphicsItem>

vpDataWidget2DGraph::vpDataWidget2DGraph(vpDataPool *datapool, QWidget *parent) :
    vpDataWidget(datapool,parent)
{
    this->setWindowTitle("2DGraph");
    this->addView(&m_graph);
    m_graph.setScene(new QGraphicsScene());
    refresh();
}


void vpDataWidget2DGraph::refresh()
{
    m_graph.scene()->clear();
    drawGraphLines();
    m_graph.fitInView(m_graph.scene()->items().last());
}

void vpDataWidget2DGraph::drawGraphLines()
{
    QVector<QPointF> datasetCuts;
    for(int f = 0 ; f < m_selectedYFields.count() ; f++)
    {
        QString yfield = m_selectedYFields[f];
        QPointF last_pos(0,0);
        for( int d = 0 ; d < m_selectedDatasets.count() ; d++)
        {
            datasetCuts.append(last_pos);
            vpDataSet* dataset = m_selectedDatasets[d];
            int yfieldindex=dataset->indexOf(yfield);
            if(yfieldindex >= 0)
            {
//COLOR!!!
                QColor color(Qt::green);//=dataset->data()->fieldColor(yfieldindex);
                for(int r = 0 ; r < dataset->getRowCount() ; r++)
                {
                    QPen pen(color);
                    QPointF new_pos;
                    new_pos.setY(dataset->getPosDouble(r,yfieldindex));
                    if(true)//(m_selectedXField.isEmpty()||m_selectedXField=="NONE")
                    {//Grafico 1D
                        new_pos.setX(last_pos.x()+1);
                    }
                    else
                    {//Grafico 2D

                    }
                    qDebug() << last_pos << new_pos;
                    m_graph.scene()->addLine(QLineF(last_pos,new_pos),pen);
                    last_pos=new_pos;
                }
            }
            else
            {//si no se encuentra el campo en el dataset
                if(m_selectedXField.isEmpty()||m_selectedXField=="NONE")
                {//Grafico 1D

                }
                else
                {//Grafico 2D

                }
            }
        }
    }
    QRectF graphrect=m_graph.sceneRect();
    m_graph.scene()->addLine(graphrect.x(),0,
                             graphrect.x()+graphrect.width(),0);
    m_graph.scene()->addLine(0,graphrect.y(),
                             0,graphrect.y()+graphrect.height());
    QRectF graph_rect = m_graph.scene()->sceneRect();
    double ymin = graph_rect.y();
    double ymax = graph_rect.y()+graph_rect.height();
    double incremento = graph_rect.height()*0.1;
    ymin-=incremento;
    ymax+=incremento;

    for(int i = 0 ; i < datasetCuts.count() ; i++)
    {
        m_graph.scene()->addLine(datasetCuts[i].x(),ymin,datasetCuts[i].x(),ymax,QPen(Qt::red));
    }
}
