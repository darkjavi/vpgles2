#include "vpgles2shader.h"

vpgles2Shader::vpgles2Shader(QGLWidget* parent, QGLShaderProgram *shader) :
    QObject(parent)
{
    m_glparent = parent;

    if(shader)
    {
        m_shader = shader;

        m_have_to_delete_shader = false;
    }
    else
    {
        init_shader();
        m_have_to_delete_shader = true;
    }
    get_shader_propierties();
}

vpgles2Shader::~vpgles2Shader()
{
    if (m_have_to_delete_shader)
    {
        delete m_shader;
    }
}
