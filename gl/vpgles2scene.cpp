#include "vpgles2scene.h"

vpGLES2Scene::vpGLES2Scene(QObject *parent) :
    QThread(parent)
{
    vp3DGeoAxis* default_axis = new vp3DGeoAxis();
    add_ojb(default_axis);
}
vpGLES2Scene::~vpGLES2Scene()
{

}

QVector4D vpGLES2Scene::draw3D(vpgles2Shader* shader)
{
    QTime timer;
    timer.start();
    ulong drawn_vertex = 0;
    ulong drawn_components = 0;
    m_lock.lock();
        for(int i = 0 ; i < m_render_pool.count() ; i++)
        {
            drawn_vertex += m_render_pool[i]->draw3D(shader);
            drawn_components += m_render_pool[i]->components().count();
        }
    m_lock.unlock();
    QVector4D scene_info(m_render_pool.count(),drawn_components,drawn_vertex,timer.elapsed());
    return scene_info;
}

ulong vpGLES2Scene::draw2D(QPainter *p)
{
    ulong drawn_vertex = 0;
    m_lock.lock();
        for(int i = 0 ; i < m_render_pool.count() ; i++)
        {
            m_render_pool[i]->draw2D(p);
        }
    m_lock.unlock();
    return drawn_vertex;
}

void vpGLES2Scene::add_ojb(vp3DGeoObj* n_obj)
{
    qDebug() << "vpGLES2Scene::add_ojb-> Añadiendo:" << n_obj->get_id() ;
    m_render_pool.append(n_obj);
    connect_obj_signals(n_obj);
    emit scene_updated();
}

//

void vpGLES2Scene::clear(bool purge_objs)
{
    m_lock.lock();
        for ( int i = 0 ; i < m_render_pool.count() ; i++)
        {
            disconnect_obj_signals(m_render_pool[i]);
            if(purge_objs)
            {
                vp3DGeoTools::delete_ojb(m_render_pool[i]);
            }
        }
    m_render_pool.clear();
    m_lock.unlock();
    vp3DGeoAxis* default_axis = new vp3DGeoAxis();
    add_ojb(default_axis);
    emit scene_updated();
}

void vpGLES2Scene::obj_updated()
{
    emit scene_updated();
}

void vpGLES2Scene::load_texture(QString id)
{
    connect(this,SIGNAL(texture_available(QString,uint))
            ,sender(),SLOT(set_tex_handle(QString,uint)));

    emit need_texture(id);
}

void vpGLES2Scene::set_tex_handle(QString tex_id, uint tex_handle)
{
    qDebug() << "vpGLES2Scene::set_tex_handle-->Recibida textura:" << tex_id << tex_handle ;
    emit texture_available(tex_id,tex_handle);
}

//aux

void vpGLES2Scene::connect_obj_signals(vp3DGeoObj *obj)
{
    connect(obj,SIGNAL(updated())
            ,this,SLOT(obj_updated()));

    connect(obj,SIGNAL(need_texture(QString))
            ,this,SLOT(load_texture(QString)));
    //connect(this,SIGNAL(texture_available(QString,uint)) //solo se conectan los que han pedido textura
    //        ,obj,SLOT(set_tex_handle(QString,uint)));


    //connect(obj,SIGNAL(destroyed()))
}

void vpGLES2Scene::disconnect_obj_signals(vp3DGeoObj *obj)
{
    qDebug()<<"vpGLES2Scene::disconnect_obj_signals-> Desconectando:" << obj->get_id();
    disconnect(obj,SIGNAL(updated())
               ,this,SLOT(obj_updated()));

    disconnect(obj,SIGNAL(need_texture(QString))
               ,this,SLOT(load_texture(QString)));

    disconnect(this,SIGNAL(texture_available(QString,uint))
               ,obj,SLOT(set_tex_handle(QString,uint)));
}
