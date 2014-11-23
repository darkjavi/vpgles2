#include "vp3dgeopart.h"

vp3DGeoPart::vp3DGeoPart()
{
    m_color = QColor(255,255,255);
    m_texture_id        = "NONE";
    m_texture_handle    = 0;
    m_text_coords.clear();
    m_vertex_array.clear();
    m_draw_mode = GL_TRIANGLES;
    m_line_size = 1;
}
