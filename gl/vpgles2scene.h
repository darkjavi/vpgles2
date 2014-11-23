#ifndef VPGLES2SCENE_H
#define VPGLES2SCENE_H

#include <QThread>
#include <QMutex>

#include "vpgles2/geo/vp3dgeoincludes.h"

class vpGLES2Scene : public QThread
{
    Q_OBJECT
public:
    explicit vpGLES2Scene(QObject *parent = 0);
    ~vpGLES2Scene();
    QVector4D draw3D(vpgles2Shader* shader);
    ulong draw2D(QPainter* p);
    void  add_ojb(vp3DGeoObj* n_obj);
    QVector<vp3DGeoObj*> objects() {return m_render_pool;}
    void  clear(bool purge_objs = true);
protected:
    QVector<vp3DGeoObj*>    m_render_pool;
    QMutex  m_lock;

    void connect_obj_signals(vp3DGeoObj* obj);
    void disconnect_obj_signals(vp3DGeoObj* obj);

signals:
    void scene_updated();
    void need_texture(QString);
    void texture_available(QString tex_id, uint tex_handle);
    void scene_info(uint objs,uint components,ulong vertex,uint time);
public slots:
    void obj_updated();
    void load_texture(QString);
    void set_tex_handle(QString tex_id, uint tex_handle);

};

#endif // VPGLES2SCENE_H
