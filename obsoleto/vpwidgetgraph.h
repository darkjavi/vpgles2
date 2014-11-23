#ifndef VPWIDGETGRAPH_H
#define VPWIDGETGRAPH_H

#include <QFrame>

#include "vpgles2widget.h"
#include "vpwidgetdata.h"


namespace Ui {
class vpWidgetGraph;
}

class vpWidgetGraph : public vpWidgetData
{
    Q_OBJECT
    
public:
    explicit vpWidgetGraph(vpDataPool* dataPool = 0,QWidget *parent = 0);
    ~vpWidgetGraph();
    void set_title(QString title, QFont font = QFont("Arial",8));
    QVector<ushort>  get_selected_indexs();
    QVector<QString> get_selected_fields();
protected:
    Ui::vpWidgetGraph *ui;
    vpGLES2Widget*  m_viewport;
    vp3DGeoGraph*   m_graph;

    QVector<QListWidgetItem*> m_fields_items;

    QGroupBox* m_gb_x_field;
    QComboBox* m_cb_x_field;
    QGroupBox* m_gb_z_field;
    QComboBox* m_cb_z_field;
    //
    void populate_listbox(vpDataSet* dataset);
    void clear_list_and_items();
    void clear_data();

signals:

private slots:
    void change_field_selection(QListWidgetItem* item);
public slots:
    void set_dataSet(vpDataSet *dataset);
    void dataSet_updated();
    void export_selected_csv();
    void set_graph_mode1d();
    void set_graph_mode2d();
    void set_graph_mode3d();
};

#endif // VPWIDGETGRAPH_H
