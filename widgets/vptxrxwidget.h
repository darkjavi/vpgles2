#ifndef VPTXRXWIDGET_H
#define VPTXRXWIDGET_H

#include <QWidget>
#include <QTimer>

class vpTxRxWidget : public QWidget
{
    Q_OBJECT
public:
    enum widgetType
    {
        WidgetProgress,
        WidgetLive,
        WidgetError
    };
    explicit vpTxRxWidget(QWidget *parent = 0);
    void set_widget_type(quint8 type);
    void set_background_color(const QColor& color);
    void set_pixmap(const QPixmap& pixmap);
protected:
    void   paintEvent(QPaintEvent *pe);
    void   mousePressEvent(QMouseEvent *event);
    quint8      m_progress;
    quint8      m_widget_type;
    QTimer      m_timer;
    quint8      m_decay_step;
    QColor      m_background;
    QColor      m_rx_color;
    QColor      m_tx_color;
    bool        m_draw_icon;
    QPixmap     m_pixmap;
signals:
private slots:
    void decay_animation();
public slots:
    void set_progress(quint8 progress);
    void tx();
    void rx();
    void error();
};
#endif // VPTXRXWIDGET_H
