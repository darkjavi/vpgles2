#include "vp3dgeograph.h"

vp3DGeoGraph::vp3DGeoGraph(QVector3D pos, QVector3D size) : vp3DGeoDataObj(pos,size)
{
    m_type       = vp3DGeoObj::TypeGraph;
    m_id         = "vp3DGeoGraph";
    m_graph_type = type_lines;
    m_graph_scale_mode = scale_relative;
    m_graph_dimensions = graph1D;
    m_graph_max_data   = 1000;
    m_graph_data_index = 0;
    m_last_update = 0;
    m_update_threshold = 1;
    generate_geometry();
    m_update_events = true;
}

QVector3D vp3DGeoGraph::get_center()
{
    return QVector3D(m_pos.x()+(m_size.x()/2),
                     m_pos.y()+(m_size.y()/2),
                     m_pos.z()+(m_size.z()/2));
}

void vp3DGeoGraph::generate_geometry()
{
    m_components.clear();
    m_x_length = m_size.x() * 0.80;
    m_y_length = m_size.y() * 0.80;
    m_x_origin = m_size.x() * 0.10;
    m_y_origin = m_size.y() * 0.10;
    generate_frame();
    generate_axis();
    generate_grid();
    generate_graph();

    if (m_update_events)
    {
        emit updated();
    }
}

void vp3DGeoGraph::generate_frame()
{
    qreal distance = -10;
    vp3DGeoPart frame;
        frame.draw_mode() = GL_TRIANGLE_FAN;
        frame.color()     = QColor(255,255,255);
        frame.text_coords() <<  QVector2D(0,0) << QVector2D(1,0) << QVector2D(1,1) << QVector2D(0,1);
        frame.vertex_array() << QVector3D(0,0,distance);
        frame.vertex_array() << QVector3D(m_size.x(),0,distance);
        frame.vertex_array() << QVector3D(m_size.x(),m_size.y(),distance);
        frame.vertex_array() << QVector3D(0,m_size.y(),distance);
        m_components.append(frame);
}

void vp3DGeoGraph::generate_axis()
{
    vp3DGeoPart axis;
        axis.draw_mode() = GL_LINES;
        axis.color()     = QColor(0,0,0);
        axis.line_size() = 2;
        //x axis
        axis.vertex_array() << QVector3D(m_x_origin,m_y_origin,0);
        axis.vertex_array() << QVector3D(m_x_origin+m_x_length,m_y_origin,0);
        //y axis
        axis.vertex_array() << QVector3D(m_x_origin,m_y_origin,m_pos.z());
        axis.vertex_array() << QVector3D(m_x_origin,m_y_origin+m_y_length,0);

    m_components.append(axis);
}

void vp3DGeoGraph::generate_grid()
{
    vp3DGeoPart grid;
        grid.draw_mode() = GL_LINES;
        grid.color()     = QColor(220,220,220);
        qreal div_size = 100;
        //x
        for ( uint div = 0 ; div <= (m_x_length /div_size) ; div++ )
        {
            qreal x_pos = m_x_origin + (div_size*div);
            grid.vertex_array() << QVector3D( x_pos , m_y_origin, 0);
            grid.vertex_array() << QVector3D( x_pos , m_y_origin+m_y_length, 0);
        }
        //y
        for ( uint div = 0 ; div <= (m_y_length /div_size) ; div++ )
        {
            qreal y_pos = m_y_origin + (div_size*div);
            grid.vertex_array() << QVector3D( m_x_origin , y_pos, 0);
            grid.vertex_array() << QVector3D( m_x_origin+m_x_length , y_pos, 0);
        }
        m_components.append(grid);
}

void vp3DGeoGraph::generate_graph()
{
    if( m_dataset )
    {
        if ( m_graph_type == type_dots )
        {
            generate_graph_dots();
        }
        else if ( m_graph_type == type_lines )
        {
            generate_graph_lines();
        }
    }
}

void vp3DGeoGraph::generate_graph_dots()
{
/*
    for ( int g = 0 ; g < m_data.size() ; g++ )
    {
        vp3DGeoPart graph;
            graph.draw_mode()   = GL_POINTS;
            graph.color()       = m_data[g].field_color();
            for ( int p = 0 ; p < m_data[g].data_array().size() ; p++ )
            {
                qreal n_x = p*m_x_length/m_data[g].data_array().size();
                qreal n_y = (m_data[g].data_array()[p] - m_data[g].data_min()) * m_y_length / (m_data[g].data_max() - m_data[g].data_min());
                graph.vertex_array() << QVector3D(n_x,n_y,10);
            }
            m_components.append(graph);
    }
*/
}

void vp3DGeoGraph::generate_graph_lines()
{
    vpDataArrayDoubles* data = (vpDataArrayDoubles*) m_dataset->data();
    if( m_graph_dimensions == graph1D )
    {
        if (m_graph_scale_mode == scale_absolute)
        {
            qreal min_y_val;
            qreal max_y_val;
            for ( int f = 0 ; f < m_selected_fields.count() ; f++ )
            {
                if (f==0)
                {
                    min_y_val = data->columnMinVal(m_selected_fields[f]);
                    max_y_val = data->columnMaxVal(m_selected_fields[f]);
                }else
                {
                    if (data->columnMinVal(m_selected_fields[f])<min_y_val)
                    {
                        min_y_val = data->columnMaxVal(m_selected_fields[f]);
                    } else if (data->columnMaxVal(m_selected_fields[f]) > max_y_val) {
                        max_y_val = data->columnMaxVal(m_selected_fields[f]);
                    }
                }
            }
            for ( int f = 0 ; f < m_selected_fields.count() ; f++ )
            {
                vp3DGeoPart graph;
                graph.draw_mode()   = GL_LINE_STRIP ;
                graph.color()       = m_dataset->data()->fieldColor(m_selected_fields[f]);
                graph.line_size()   = 2;
                for ( uint i = 0 ; i < m_graph_max_data && i+m_graph_data_index < m_dataset->data()->columnToDoubles(m_selected_fields[f]).count() ; i++ )
                {
                    qreal n_x = 0;
                    qreal n_y = 0;
                    n_x = i*m_x_length/m_graph_max_data;
                    n_y = (m_dataset->data()->columnToDoubles(m_selected_fields[f])[i+m_graph_data_index] - min_y_val ) * m_y_length / (max_y_val - min_y_val);
                    graph.vertex_array() << QVector3D(n_x+m_x_origin,n_y+m_y_origin,10);
                }
                m_components.append(graph);
            }
        }else if (m_graph_scale_mode == scale_relative)
        {
            for ( int f = 0 ; f < m_selected_fields.count() ; f++ )
            {
                vp3DGeoPart graph;
                graph.draw_mode()   = GL_LINE_STRIP ;
                graph.color()       = m_dataset->data()->fieldColor(m_selected_fields[f]);
                graph.line_size()   = 2;
                for ( int i = 0 ; i < m_graph_max_data && i+m_graph_data_index < m_dataset->data()->columnToDoubles(m_selected_fields[f]).count() ; i++ )
                {
                qreal n_x = 0;
                qreal n_y = 0;
                n_x = i*m_x_length/m_graph_max_data;
                n_y = (m_dataset->data()->columnToDoubles(m_selected_fields[f])[i+m_graph_data_index]) * m_y_length / (data->columnMaxVal(m_selected_fields[f]));
                graph.vertex_array() << QVector3D(n_x+m_x_origin,n_y+m_y_origin,10);
                }
                m_components.append(graph);
            }
        }
    }else if (m_graph_dimensions == graph2D)
    {
        if (m_graph_scale_mode == scale_absolute)
        {
        }else if (m_graph_scale_mode == scale_relative)
        {
        }
    }else if (m_graph_dimensions == graph3D)
    {
        if (m_graph_scale_mode == scale_absolute)
        {
        }else if (m_graph_scale_mode == scale_relative)
        {
        }
    }
}


//SLOTS

void vp3DGeoGraph::set_scale_absolute()
{
    m_graph_scale_mode = scale_absolute;
    update();
}

void vp3DGeoGraph::set_scale_relative()
{
    m_graph_scale_mode = scale_relative;
    update();
}
