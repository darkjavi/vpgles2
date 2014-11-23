#include "vpgles2textureloader.h"

#include <QLabel>

vpGLES2TextureLoader::vpGLES2TextureLoader(QGLWidget *parent) :
    QThread(parent)
{
    m_glwidget              = parent;
    m_running               = false;
    m_tex_loading_id        = DEFAULT_TEX_LOADING;
    m_tex_error_id          = DEFAULT_TEX_ERROR;
    m_tex_loading_handle    = m_glwidget->bindTexture(QPixmap(":/icons/loading.jpg"),GL_TEXTURE_2D);
    m_tex_error_handle      = m_glwidget->bindTexture(QPixmap(":/icons/error.png"),GL_TEXTURE_2D);
}

vpGLES2TextureLoader::~vpGLES2TextureLoader()
{
    m_running = false;
}

void vpGLES2TextureLoader::run()
{
    qDebug() << "[INFO]vpGLES2TextureLoader::run-> arrancando thread" ;
    m_running = true;
    while( (m_load_queue.count() > 0) && m_running )
    {
        qDebug() << "[DEBUG]vpGLES2TextureLoader::run-> procesando:" << m_load_queue[0] ;
        long tex_handle = get_text(m_load_queue[0]);
        if ( tex_handle != -1 )
        {
            emit texture_available(m_load_queue[0],tex_handle);
        }
        else
        {
            qDebug() << "[WARN]vpGLES2TextureLoader::run-> No he podido cargar la textura:" << m_load_queue[0] ;
            //emit texture_available(m_load_queue[0],m_tex_error_handle);
        }
        m_load_queue.remove(0);
    }
    m_running = false;
    qDebug() << "[INFO]vpGLES2TextureLoader::run-> parando thread" ;
}


long vpGLES2TextureLoader::get_text(QString tex_id)
{
    return -1;//Implementame!
}




void vpGLES2TextureLoader::load_texture(QString tex_id)
{
    qDebug() << "[DEBUG]vpGLES2TextureLoader::load_texture-> tex solicitada:" << tex_id ;
    if(! m_running)
    {
        this->start();
    }
    if (m_tex_loading_id != "NONE")
    {
        emit texture_available(tex_id,m_tex_loading_handle);
    }
    m_load_queue.append(tex_id);
}
