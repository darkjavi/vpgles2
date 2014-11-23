#include "vpgles2widget.h"

vpGLES2Widget::vpGLES2Widget(QWidget *parent) : QGLWidget(parent)
{
    m_shader_props  = 0;
    m_tex_loader    = 0;

    m_clearColor = QColor::fromRgb(50,50,50);
    m_frames = 0;
    m_show_debug = true;
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoBufferSwap(false);

    setMouseTracking(true);

    m_time.start();
    m_hudEnabled =false;

}

vpGLES2Widget::~vpGLES2Widget()
{
    if (m_shader_props)
    {
        delete m_shader_props;
    }
    if (m_tex_loader)
    {
        delete m_tex_loader;
    }
}
//--------------------------------------------------------------------------
//GL cosas
void vpGLES2Widget::initializeGL()
{
    dump_glinfo();
    m_render_lock.lock();
        qglClearColor(m_clearColor);
        m_shader_props = new vpgles2Shader(this);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        m_tex_loader = new vpGLES2TextureLoader(this);
        glFrontFace(GL_CW);
        glCullFace(GL_FRONT);
        //glEnable(GL_CULL_FACE); // Esto hace que solo se vean los poligonos por un lado!!!!
        //glEnable(GL_COLOR_MATERIAL);
        //glShadeModel(GL_SMOOTH);
        m_shader_props->shader()->setUniformValue(m_shader_props->textureUniform(), 0);
        glBindTexture(GL_TEXTURE_2D,0);
    m_render_lock.unlock();

    //ojo aqui! quien borra la camara geometrica(la propia camara o la escena!)

    connect(this,SIGNAL(render_time(uint)),&m_camera,SLOT(set_shuttler_time(uint)));
    connect_scene(&m_scene);
    connect(&m_camera,SIGNAL(camera_updated()),this,SLOT(updateGL()));
    connect(&m_hud,SIGNAL(updated()),&m_camera,SLOT(update()));
    //m_scene.add_ojb(m_camera.camera_representation());
    m_camera.look_at(QVector3D(0,0,0));
    //m_camera.set_movemode_arround(m_scene.objects().first());
}

void vpGLES2Widget::paintGL()
{
    QTime render_timer;
    render_timer.start();
    m_render_lock.lock();
        QPainter painter;
        painter.begin(this);
        painter.beginNativePainting();

        qglClearColor(m_clearColor);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        m_shader_props->bind_shader();
            m_shader_props->set_modelview(m_camera.modelview());
            // DIBUJAR GEOMETRIA 3D AQUI!!
            QVector4D scene_info = m_scene.draw3D(m_shader_props);
        m_shader_props->release_shader();

        //Dibujamos el 2d
        glDisable(GL_DEPTH_TEST);
        painter.endNativePainting();
        m_scene.draw2D(&painter);
        if(m_hudEnabled)m_hud.draw(painter);
        qreal fps = m_frames /(m_time.elapsed() / 1000.0);

        if(m_show_debug)
        {
            QString debug_info;
            debug_info.append(" o:");
            debug_info.append(QString::number(scene_info.x(),'f',0));

            debug_info.append(" c:");
            debug_info.append(QString::number(scene_info.y(),'f',0));

            debug_info.append(" v:");
            debug_info.append(QString::number(scene_info.z(),'f',0));

            debug_info.append(" st:");
            debug_info.append(QString::number(scene_info.w(),'f',0));

            debug_info.append(" vt:");
            debug_info.append(QString::number(render_timer.elapsed(),'f',0));

            debug_info.append(" fps:");
            debug_info.append(QString::number(fps, 'f', 2));

            painter.setPen(Qt::blue);
            painter.drawText(10,20,debug_info);
            //renderText(10,20,debug_info);ROTO en opengles!
            //renderText(20,30,20,debug_info,QFont("ARIAL",200));
        }

        painter.end();
        swapBuffers();
    m_render_lock.unlock();


    if (!(m_frames % 10)) {
    m_time.start();
    m_frames = 0;
    }
    m_frames ++;

    int render_ms = render_timer.elapsed();
    emit performance_info(scene_info.w(),scene_info.x(),scene_info.y(),scene_info.z(),render_ms);
    emit render_time(render_ms);
}

void vpGLES2Widget::resizeGL(int w, int h)
{
    m_camera.set_aspectRadio((qreal)w/(qreal)h);
}

uint vpGLES2Widget::draw_geo_obj(vp3DGeoObj &geoObj)
{
    ulong drawn_vertex = 0;
    for ( int part = 0 ; part < geoObj.components().count() ; part++ )
    {
        if ( geoObj.components()[part].draw_mode() == GL_POINTS )
        {
            //glPointSize(geoObj.components()[part].line_size());
        }
        else
        {
            glLineWidth(geoObj.components()[part].line_size());
        }


        if ( geoObj.components()[part].texture_id() == "NONE" )
        {
            m_shader_props->disable_texCoordArray();
            glBindTexture(GL_TEXTURE_2D,0);
            qglColor(geoObj.components()[part].color());
        }
        else
        {
            m_shader_props->enable_texCoordArray();;
            glBindTexture(GL_TEXTURE_2D,geoObj.components()[part].texture_handle());
            m_shader_props->shader()->setAttributeArray(m_shader_props->texCoordAttr(), geoObj.components()[part].text_coords().constData(), 2);
            m_shader_props->shader()->setUniformValue(m_shader_props->textureUniform(), 0);
        }
        m_shader_props->enable_vertexArray();
        m_shader_props->shader()->setAttributeArray(m_shader_props->vertexAttr(),geoObj.components()[part].vertex_array().constData());
        drawn_vertex += geoObj.components()[part].vertex_array().size();
        glDrawArrays(geoObj.components()[part].draw_mode(), 0, geoObj.components()[part].vertex_array().size());
    }
    return drawn_vertex;
}

void vpGLES2Widget::set_clearColor(QColor n_color)
{
    m_clearColor = n_color;
}

void vpGLES2Widget::connect_scene(vpGLES2Scene *n_scene)
{
    connect(n_scene,SIGNAL(need_texture(QString))
            ,m_tex_loader,SLOT(load_texture(QString)));

    connect(m_tex_loader,SIGNAL(texture_available(QString,uint))
            ,n_scene,SLOT(set_tex_handle(QString,uint)));

    connect(n_scene,SIGNAL(scene_updated())
            ,&m_camera,SLOT(update()));
}

void vpGLES2Widget::disconnect_scene(vpGLES2Scene *n_scene)
{
    disconnect(n_scene,SIGNAL(need_texture(QString))
               ,m_tex_loader,SLOT(load_texture(QString)));

    disconnect(m_tex_loader,SIGNAL(texture_avaiable(QString,uint))
               ,n_scene,SLOT(set_tex_handle(QString,uint)));
    disconnect(n_scene,SIGNAL(scene_updated())
               ,&m_camera,SLOT(scene_changed()));
}
//---------------------------------------------------------------------------
//manejo de eventos
void vpGLES2Widget::keyPressEvent(QKeyEvent *event)
{
    m_camera.keyPressEvent(event);
}

void vpGLES2Widget::mouseMoveEvent(QMouseEvent *event)
{
    m_camera.mouseMoveEvent(event);
    //m_hud.mouse_in(event);
}

void vpGLES2Widget::mousePressEvent(QMouseEvent *event)
{
    m_camera.mousePressEvent(event);
    //m_hud.mouse_in(event);
}

void vpGLES2Widget::wheelEvent(QWheelEvent *event)
{
    m_camera.wheelEvent(event);
}

void vpGLES2Widget::dump_glinfo()
{
    {
        const GLubyte *renderer    = glGetString( GL_RENDERER );
        const GLubyte *vendor      = glGetString( GL_VENDOR );
        const GLubyte *version     = glGetString( GL_VERSION );
        const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

//        GLint major, minor;
//        glGetIntegerv(GL_MAJOR_VERSION, &major);
//        glGetIntegerv(GL_MINOR_VERSION, &minor);
        qDebug("[INFO]vpGLES2Widget-> OPENGL INFO:");
        qDebug("GL Vendor     : %s", vendor);
        qDebug("GL Renderer   : %s", renderer);
        qDebug("GL Version(s) : %s", version);
//        qDebug("GL Version(i) : %d.%d", major, minor);
        qDebug("GLSL Version  : %s\n", glslVersion);
    }
}


//Benchmark mode
void vpGLES2Widget::start_benchmark()
{
    m_scene.clear();
    m_camera.set_shuttler_maxfps(200);
    m_camera.set_shuttler_mode_auto();
    connect(&m_camera,SIGNAL(camera_updated()),this,SLOT(benchmark()));
}

void vpGLES2Widget::stop_benchmark()
{
    m_scene.clear();
    m_camera.set_shuttler_maxfps(vpTCamera::CamShutterDefaultMaxFPS);
    m_camera.set_shuttler_mode_events();
    disconnect(&m_camera,SIGNAL(camera_updated()),this,SLOT(benchmark()));
}

void vpGLES2Widget::benchmark()
{

}
