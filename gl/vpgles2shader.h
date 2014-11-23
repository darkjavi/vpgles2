#ifndef VPGLES2SHADER_H
#define VPGLES2SHADER_H

#include <QObject>
#include <QGLShaderProgram>
#include <QGLWidget>
//#include  "vpgles2widget.h"

class vpgles2Shader : public QObject
{
    Q_OBJECT
public:
    explicit vpgles2Shader(QGLWidget* parent, QGLShaderProgram* shader = 0);
    ~vpgles2Shader();

    QGLShaderProgram* shader()  {return m_shader;}
    int vertexAttr()            {return m_vertexAttr;}
    int normalAttr()            {return m_normalAttr;}
    int texCoordAttr()          {return m_texCoordAttr;}
    int matrixUniform()         {return m_matrixUniform;}
    int textureUniform()        {return m_textureUniform;}
    int colorUniform()          {return m_colorUniform;}

    void init_shader()
    {
        m_shader = new QGLShaderProgram();
        QGLShader *vshader1 = new QGLShader(QGLShader::Vertex);
        vshader1->compileSourceFile(":/shaders/ShaderVTest.vsh");

        QGLShader *fshader1 = new QGLShader(QGLShader::Fragment);
        fshader1->compileSourceFile(":/shaders/ShaderFTest.fsh");

        m_shader->addShader(vshader1);
        m_shader->addShader(fshader1);
        m_shader->link();
    }

    void set_modelview(QMatrix4x4& modelview)
    {
        m_shader->setUniformValue(m_matrixUniform, modelview);
    }

    void bind_shader()
    {
        m_shader->bind();
    }

    void release_shader()
    {
        m_shader->release();
    }

    void enable_normalsArray()
    {
        m_shader->enableAttributeArray(m_normalAttr);
    }

    void disable_normalsArray()
    {
        m_shader->disableAttributeArray(m_normalAttr);
    }

    void enable_vertexArray()
    {
        m_shader->enableAttributeArray(m_vertexAttr);
    }

    void disable_vertexArray()
    {
        m_shader->disableAttributeArray(m_vertexAttr);
    }

    void enable_texCoordArray()
    {
        m_shader->enableAttributeArray(m_texCoordAttr);
    }

    void disable_texCoordArray()
    {
        m_shader->disableAttributeArray(m_texCoordAttr);
    }

    void set_uniform_value(int)
    {

    }

    void makecurrent()
    {
        m_glparent->makeCurrent();
    }

    void qglColor(QColor color)
    {
        m_glparent->qglColor(color);
    }

    void renderText(qreal x, qreal y, qreal z, const QString &str,const QFont &fnt)
    {
        m_glparent->renderText(x,y,z,str,fnt);
    }

private:
    QGLShaderProgram*   m_shader;
    QGLWidget*          m_glparent;
    bool    m_have_to_delete_shader;
    int m_vertexAttr;
    int m_normalAttr;
    int m_texCoordAttr ;
    int m_matrixUniform;
    int m_textureUniform;
    int m_colorUniform;
    void get_shader_propierties()
    {
        m_vertexAttr     = m_shader->attributeLocation("vertex");
        m_normalAttr     = m_shader->attributeLocation("normal");
        m_texCoordAttr   = m_shader->attributeLocation("texCoord");
        m_matrixUniform  = m_shader->uniformLocation("matrix");
        m_textureUniform = m_shader->uniformLocation("tex");
        m_colorUniform   = m_shader->uniformLocation("solid_color");
    }
signals:
    
public slots:
    
};

#endif // VPGLES2SHADER_H
