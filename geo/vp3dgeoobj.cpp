#include "vp3dgeoobj.h"

vp3DGeoObj::vp3DGeoObj(QVector3D pos,QVector3D size,QColor color) : QObject()
{
    m_type      = vp3DGeoObj::TypeUnkown;
    m_id        = "Unkown";
    m_pos       = pos;
    m_size      = size;
    m_color     = color;
    m_visible       = true;
    m_draw_lines    = false;
    m_draw_points   = false;
    m_draw_faces    = true;
    m_update_events = false;
}

ulong vp3DGeoObj::draw3D(vpgles2Shader *shader)
{
    if (!m_visible)
    {
        return 0;
    }
    ulong drawn_vertex = 0;
    for ( int part = 0 ; part < m_components.count() ; part++ )
    {
        if ( m_draw_points)
            drawn_vertex += draw_part_points(m_components[part],shader);

        if ( m_draw_lines )
            drawn_vertex += draw_part_lines(m_components[part],shader);

        if ( m_draw_faces )
        {
            if ( m_components[part].texture_id() == "NONE" )
            {//pintamos sin textura
                drawn_vertex += draw_part_solid(m_components[part],shader);
            }
            else
            {//pintamos con textura
                drawn_vertex += draw_part_textured(m_components[part],shader);
            }
        }
    }
    return drawn_vertex;
}

ulong vp3DGeoObj::draw_part_points(vp3DGeoPart& part,vpgles2Shader* shader)
{
    //glPointSize(2.0);
    shader->enable_vertexArray();

        shader->qglColor(part.color().lighter());
        shader->shader()->setAttributeArray(shader->vertexAttr(),part.vertex_array().constData());
        glDrawArrays(GL_POINTS, 0, part.vertex_array().count());

    shader->disable_vertexArray();
    return part.vertex_array().count();
}

ulong vp3DGeoObj::draw_part_lines(vp3DGeoPart& part,vpgles2Shader* shader)
{
    glLineWidth(2.0);
    shader->enable_vertexArray();

        shader->qglColor(part.color().lighter());
        shader->shader()->setAttributeArray(shader->vertexAttr(),part.vertex_array().constData());
        glDrawArrays(GL_LINE_STRIP, 0, part.vertex_array().count());

    shader->disable_vertexArray();
    return part.vertex_array().count();
}

ulong vp3DGeoObj::draw_part_solid(vp3DGeoPart& part,vpgles2Shader* shader)
{

    shader->enable_vertexArray();
        glLineWidth(part.line_size());
        //glPointSize(part.line_size());
        QVector4D color(part.color().redF(),
                        part.color().greenF(),
                        part.color().blueF(),
                        part.color().alphaF());
        shader->shader()->setUniformValue(shader->colorUniform(),color);
        shader->shader()->setAttributeArray(shader->vertexAttr(),part.vertex_array().constData());
        glDrawArrays(part.draw_mode(), 0, part.vertex_array().count());
    shader->disable_vertexArray();
    return part.vertex_array().count();
}

ulong vp3DGeoObj::draw_part_textured(vp3DGeoPart& part,vpgles2Shader* shader)
{
    shader->enable_vertexArray();
    shader->enable_texCoordArray();
    //shader->qglColor(QColor(255,255,255));
        if (part.texture_handle() == 0)
        {// si la textura no tiene id, la pedimos
            emit need_texture(part.texture_id());
        }
        if (part.text_coords().count() == 0)
        {
            qDebug() << "[WARN!]vp3DGeoObj::draw->No hay textcoords para la textura: " << part.texture_id() << ". Me inventare algunas";
            part.text_coords() << QVector2D(0,0) << QVector2D(1,0) << QVector2D(1,1) << QVector2D(0,1);
        }

        shader->shader()->setAttributeArray(shader->vertexAttr(),part.vertex_array().constData());
        shader->shader()->setAttributeArray(shader->texCoordAttr(),part.text_coords().constData(),2);

        glBindTexture(GL_TEXTURE_2D,part.texture_handle());
        glDrawArrays(part.draw_mode(), 0, part.vertex_array().count());
    glBindTexture(GL_TEXTURE_2D,0);
    shader->disable_texCoordArray();
    shader->disable_vertexArray();
    return part.vertex_array().count();

}

ulong vp3DGeoObj::draw2D(QPainter *p)
{
    return 0;
}

void vp3DGeoObj::set_tex_handle(QString tex_id, uint tex_handle)
{
    bool something_updated = false;
    for (int i = 0 ; i < m_components.count() ; i++)
    {
        if (m_components[i].texture_id() == tex_id)
        {
            m_components[i].texture_handle() = tex_handle;
            something_updated = true;
            qDebug() << "vp3DGeoObj::set_tex_handle-->Recibida textura:" << tex_id << tex_handle ;
        }
    }
    if ( something_updated )
    {
        emit updated();
    }
}
