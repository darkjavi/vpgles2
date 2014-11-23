#ifndef VPDOCKWIDGETTEST_H
#define VPDOCKWIDGETTEST_H

#include <QDockWidget>

#include "vpgles2widget.h"

namespace Ui {
class vpDockWidgetTest;
}

class vpDockWidgetTest : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit vpDockWidgetTest(QWidget *parent = 0);
    ~vpDockWidgetTest();
    
protected:
    Ui::vpDockWidgetTest *ui;
    vpGLES2Widget*  m_glwidget;

    bool m_benchmarking;
signals:

public slots:
    void benchmark();
    void collect_benchmark_info(ulong objs,ulong components,ulong vertex,ulong scene_time,ulong vp_time);
};

#endif // VPDOCKWIDGETTEST_H
