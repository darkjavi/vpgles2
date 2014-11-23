#ifndef VP3DGEOGRAPH_H
#define VP3DGEOGRAPH_H

#include "vp3dgeodataobj.h"

class vp3DGeoGraph : public vp3DGeoDataObj
{
    Q_OBJECT
public:
    enum scaleModes
    {
        scale_relative,
        scale_absolute
    };
    enum graphType
    {
        type_dots,
        type_lines,
        type_bezier
    };
    enum grapDimensions
    {
        graph1D,
        graph2D,
        graph3D
    };

    vp3DGeoGraph(QVector3D pos, QVector3D size);
    QVector3D get_center();

protected:
    qint8   m_graph_type;
    quint8  m_graph_scale_mode;
    quint8  m_graph_dimensions;
    quint8  m_update_threshold;
    quint8  m_last_update;
    qreal   m_x_origin;
    qreal   m_x_length;

    qreal   m_y_origin;
    qreal   m_y_length;

    uint    m_graph_max_data;
    uint    m_graph_data_index;


    void generate_geometry();
    void generate_frame();
    void generate_axis();
    void generate_grid();
    void generate_graph();
    void generate_graph_dots();
    void generate_graph_lines();
signals:

public slots:
    void set_scale_absolute();
    void set_scale_relative();
};

#endif // VP3DGEOGRAPH_H
