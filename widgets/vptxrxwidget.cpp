#include "vptxrxwidget.h"
#include <QPainter>
#include <QDebug>

vpTxRxWidget::vpTxRxWidget(QWidget *parent) :
    QWidget(parent)
{
    m_progress = 0;
    m_background  = QColor(0,0,100,150);
    m_rx_color    = QColor(0,255,0,0);
    m_tx_color    = QColor(255,0,0,0);
    m_timer.setInterval(50);
    m_decay_step  = 25;
    m_draw_icon   = false;
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(decay_animation()));
    m_widget_type = vpTxRxWidget::WidgetLive;
}
void vpTxRxWidget::paintEvent(QPaintEvent *pe)
{
    QWidget::paintEvent(pe);
    QPainter p(this);
    QColor background(m_background);
    background.setAlpha(255);
    p.setPen(QColor(background));
    QRect rect(0,0,this->width(),this->height());
    rect.setSize(rect.size()-QSize(1,1));
    p.drawRect(rect);

    if(m_widget_type == WidgetProgress)
    {
        QRect completed_rect = rect;
        completed_rect.setWidth(completed_rect.width()*(m_progress/100.0f));
        p.setPen(QColor(0,0,0,0));
        p.setBrush(m_background);
        p.drawRect(completed_rect);
        if (m_draw_icon) p.drawPixmap(rect,m_pixmap);
    }
    else if (m_widget_type == WidgetLive)
    {
        p.setPen(QColor(0,0,0,0));
        p.setBrush(m_background);
        p.drawRect(rect);
        if (m_draw_icon) p.drawPixmap(rect,m_pixmap);
        QPoint center = rect.center();
        QPoint l_center(center);
        QPoint r_center(center);
        l_center.setX(l_center.x()/2);
        r_center.setX(center.x()+ l_center.x());
        quint8 radius = l_center.x()*0.9;
        QColor rx_color(m_rx_color);
        rx_color.setAlpha(255);
        p.setPen(rx_color);
        p.setBrush(m_rx_color);
        p.drawEllipse(l_center,radius,radius);

        QColor tx_color(m_tx_color);
        tx_color.setAlpha(255);
        p.setPen(tx_color);
        p.setBrush(m_tx_color);
        p.drawEllipse(r_center,radius,radius);
    }
    else if(m_widget_type==WidgetError)
    {
        p.drawImage(this->rect(),QImage(":/icons/error.png"));
    }
}

void vpTxRxWidget::mousePressEvent(QMouseEvent *)
{
    tx();
}

void vpTxRxWidget::set_widget_type(quint8 type)
{
    m_widget_type = type;
    update();
}

void vpTxRxWidget::set_background_color(const QColor& color)
{
    m_background = color;
    update();
}

void vpTxRxWidget::set_pixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    m_draw_icon = true;
    update();
}

//SLOTS

void vpTxRxWidget::set_progress(quint8 progress)
{
    qDebug()<<"Progress"<<progress;
    m_widget_type = vpTxRxWidget::WidgetProgress;
    if (progress > 100) progress = 100;
    m_progress = progress;
    update();
}

void vpTxRxWidget::tx()
{
    m_tx_color.setAlpha(250);
    m_timer.start();
    update();
}

void vpTxRxWidget::rx()
{
    m_widget_type = vpTxRxWidget::WidgetLive;
    m_rx_color.setAlpha(250);
    m_timer.start();
    update();
}

void vpTxRxWidget::error()
{
    m_widget_type = WidgetError;
}

void vpTxRxWidget::decay_animation()
{
    if((m_rx_color.alpha() == 0) && (m_tx_color.alpha() == 0))
    {
        m_timer.stop();
        return;
    }
    if(m_rx_color.alpha() > 0)
    {
        m_rx_color.setAlpha(m_rx_color.alpha()-m_decay_step);
    }

    if(m_tx_color.alpha() > 0)
    {
        m_tx_color.setAlpha(m_tx_color.alpha()-m_decay_step);
    }
    update();
}
