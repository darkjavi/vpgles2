#ifndef VPGLES2TEXTURELOADER_H
#define VPGLES2TEXTURELOADER_H

#include <QThread>
#include <QGLWidget>
#include <QDebug>

#define DEFAULT_TEX_LOADING "loading.png";
#define DEFAULT_TEX_ERROR   "error.png";

class vpGLES2TextureLoader : public QThread
{
    Q_OBJECT
public:
    explicit vpGLES2TextureLoader(QGLWidget *parent);
    ~vpGLES2TextureLoader();
protected:
    QVector<QString> m_load_queue;

    QVector<QString> m_text_pool_ids;
    QVector<QString> m_text_pool_md5;
    QVector<uint>    m_text_pool_handles;

    bool     m_running;
    uint     m_tex_loading_handle;
    QString  m_tex_loading_id;
    uint     m_tex_error_handle;
    QString  m_tex_error_id;
    QGLWidget*  m_glwidget;

    long get_text(QString tex_id);
    void run();
signals:
    void texture_available(QString tex_id,uint tex_handle);
public slots:
    void load_texture(QString tex_id);
};

#endif // VPGLES2TEXTURELOADER_H
