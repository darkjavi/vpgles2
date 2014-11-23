#ifndef VP3DGEOOBJ_H
#define VP3DGEOOBJ_H

#include "vp3dgeopart.h"
#include "vpgles2shader.h"


class vp3DGeoObj : public QObject
{
    Q_OBJECT
public:
    enum GeoType
    {
        TypeUnkown,
        TypeAxis,
        TypePrimitive,
        TypeGraph
    };

    vp3DGeoObj(QVector3D pos,QVector3D size,QColor color = QColor(255,0,0));

    QVector<vp3DGeoPart>& components(){return m_components;}

    bool is_visible()               {return m_visible;}
    void set_visible(bool visible)  {m_visible = visible;}
    bool toggle_visible()           {m_visible = !m_visible;return m_visible;}

    void set_udpate_events(bool send_events) {m_update_events = send_events;}
    bool toggle_update_events()     {m_update_events = !m_update_events;return m_update_events;}

    void set_id(QString n_id)       {m_id = n_id;}
    QString get_id()                {return m_id;}
    quint8    type()                {return m_type;}
    QVector3D get_size()            {return m_size;}

    ulong draw3D(vpgles2Shader* shader);
    virtual ulong draw2D(QPainter* p);
    virtual QVector3D get_center(){return m_pos;}
protected:
    QString     m_id;
    qint8       m_type;
    QVector3D   m_pos;
    QVector3D   m_size;
    bool        m_visible;
    bool        m_draw_points;
    bool        m_draw_lines;
    bool        m_draw_faces;
    QColor      m_color;
    QVector<vp3DGeoPart> m_components;
    bool m_update_events;
    ulong draw_part_points  (vp3DGeoPart& part,vpgles2Shader* shader);
    ulong draw_part_lines   (vp3DGeoPart& part,vpgles2Shader* shader);
    ulong draw_part_solid   (vp3DGeoPart& part,vpgles2Shader* shader);
    ulong draw_part_textured(vp3DGeoPart& part,vpgles2Shader* shader);
    virtual void generate_geometry() = 0;


signals:
    void updated();
    void need_texture(QString tex_id);
public slots:
    void set_tex_handle(QString tex_id, uint tex_handle);
};

#endif // VP3DGEOOBJ_H
