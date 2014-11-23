#include "vp2dsimplegraph.h"
#include "ui_vp2dsimplegraph.h"

#include <QPainter>
#include <QDebug>

vp2DSimpleGraph::vp2DSimpleGraph(QWidget *parent, QVector<QString> fields, quint16 maxdata) :
    QFrame(parent),
    ui(new Ui::vp2DSimpleGraph)
{
    ui->setupUi(this);
    setFields(fields);
    m_maxData=maxdata;
    m_draggingEvent=false;
}

vp2DSimpleGraph::~vp2DSimpleGraph()
{
    delete ui;
}

void vp2DSimpleGraph::addMark(QColor c)
{
    m_marks.append(m_data[0].count());
    m_marksColors.append(c);
}

void vp2DSimpleGraph::setFields(QVector<QString> fields)
{
    m_fields=fields;
    m_data.clear();
    m_colors.clear();
    for (int i = 0 ; i < fields.count() ; i++)
    {
        m_data.append(QVector<double>());
        m_colors.append(QColor(qrand()%255,qrand()%255,qrand()%255));
    }
    repaint();
}

void vp2DSimpleGraph::appendData(QVector<double> doubles)
{
    if(doubles.count()!=m_data.count())
    {
        qDebug()<<"[ERROR]vp2DSimpleGraph::appendData-> unexpected data length! "<<doubles.count()<<"/"<<m_data.count();
        return;
    }
    while(m_data[0].count()>=m_maxData)
    {
        for(int i = 0 ; i < m_data.count() ; i++)
        {
            m_data[i].remove(0);
        }
        for(int i=0;i<m_marks.count();i++)
        {
            if(i>=m_marks.count())break;
            m_marks[i]--;
            if(m_marks[i]<=0)
            {
                m_marks.remove(i);
                m_marksColors.remove(i);
            }
        }
    }
    for(int i = 0 ; i < m_data.count() ; i++)
    {
        m_data[i].append(doubles[i]);
    }
    repaint();
}

void vp2DSimpleGraph::paintEvent(QPaintEvent *pe)
{
    QFrame::paintEvent(pe);
    QPainter p(this);
    draw_graph(&p,this->rect());
}

void vp2DSimpleGraph::draw_graph(QPainter *p, QRect rect)
{
    p->setPen(Qt::black);
    p->setBrush(Qt::white);
    p->drawRoundedRect(rect,5,5);
    quint16 x_origin=rect.width()*0.1;
    quint16 x_length=rect.width()-x_origin;
    qint16 y_origin=rect.height()*0.9;
    qint16 y_length=y_origin;
    QRect graph_rect(x_origin,0,x_length,y_length);
    p->drawRect(graph_rect);
    //dibujar los datos
    for(int f = 0 ; f<m_data.count() ; f++)
    {
        p->setPen(QPen(m_colors[f]));

        //hallar los maximos y minimos:
        qreal x_max=m_maxData,x_min=0;
        qreal y_max=0,y_min=0;
        for(int r = 0 ; r<m_data[f].count() ;r++)
        {
            if(r==0) {y_max=m_data[f][r];y_min=m_data[f][r];}
            if     (m_data[f][r]<y_min) y_min=m_data[f][r];
            else if(m_data[f][r]>y_max) y_max=m_data[f][r];
        }
        //pintar el grafico
        qreal last_x=x_origin;
        qreal last_y=y_origin;
        for(int r = 0 ; r<m_data[f].count() ;r++)
        {
            qreal px=r*x_length/x_max-x_min;
            px+=x_origin;
            qreal py=(m_data[f][r] - y_min) * y_length / (y_max-y_min);
            py=y_origin-py;
            p->drawLine(last_x,last_y,px,py);
            last_x=px;
            last_y=py;
        }
        //
        //pintamos las marcas
        for(int m=0;m<m_marks.count();m++)
        {
            p->setPen(QPen(m_marksColors[m]));
            qreal px=m_marks[m]*x_length/x_max-x_min;
            px+=x_origin;
            p->drawLine(px,0,px,rect.height());
        }
        //pintamos los textos
        p->drawText(QRect(0,f*y_length/m_fields.count(),x_origin,abs(y_length/m_fields.count())),m_fields[f]);
    }
}

void vp2DSimpleGraph::mousePressEvent(QMouseEvent *e)
{
    m_lastMX=e->x();
    m_lastMY=e->y();
}

void vp2DSimpleGraph::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()==Qt::LeftButton)
    {
        //qint16 d_x= m_lastMX-e->x();
        //qint16 d_y= m_lastMY-e->y();
        //qDebug() << "d_x:" << d_x << "d_y:" << d_y;
    }
    m_lastMX=e->x();
    m_lastMY=e->y();
}

void vp2DSimpleGraph::mouseReleaseEvent(QMouseEvent*)
{

}
