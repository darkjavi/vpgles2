#ifndef VPGLES2WIDGET_H
#define VPGLES2WIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QTime>
#include <QTimer>
#include <QVector>

#include <QPainter>
#include <QPaintEngine>
#include <math.h>

#include "vpgles2shader.h"
#include "vpgles2textureloader.h"
#include "vpgles2scene.h"
#include "vptcamera.h"
#include "vphud.h"
#include "vp3dgeoincludes.h"

class vpGLES2Widget : public QGLWidget
{
    Q_OBJECT
public:
    vpGLES2Widget(QWidget *parent = 0);
    ~vpGLES2Widget();

    void set_clearColor(QColor n_color);

    vpGLES2Scene& scene()   {return m_scene;}
    vpTCamera&    camera()  {return m_camera;}

    void dump_glinfo();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent      (QKeyEvent *event);
    void mousePressEvent    (QMouseEvent *event);
    void mouseMoveEvent     (QMouseEvent *event);
    void wheelEvent         (QWheelEvent *event );

    uint draw_geo_obj    (vp3DGeoObj& geoObj);
    void connect_scene   (vpGLES2Scene* n_scene);
    void disconnect_scene(vpGLES2Scene* n_scene);

private:
    QColor  m_clearColor;
    int     m_frames;
    QTime   m_time;
    bool    m_show_debug;
    vpTCamera       m_camera;
    vpGLES2Scene    m_scene;
    vpHud           m_hud;

    QMutex m_render_lock;

    vpGLES2TextureLoader *m_tex_loader;
    vpgles2Shader   *m_shader_props;

    uint m_bench_frames;
signals:
    void render_time(uint msecs);
    void performance_info(ulong objs,ulong components,ulong vertex,ulong scene_time,ulong vp_time);
private slots:
    void benchmark();
public slots:
    void start_benchmark();
    void stop_benchmark();
};

#endif // VPGLES2WIDGET_H
