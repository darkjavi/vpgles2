#ifndef VPGRAPHWIDGET_H
#define VPGRAPHWIDGET_H

#include <QWidget>

#include "vpdatacontainer.h"
#include "vpgles2widget.h"

class vpGraphWidget : public QFrame
{
    Q_OBJECT
public:
    explicit vpGraphWidget(QWidget *parent = 0);
    ~vpGraphWidget();
private:
    vpGLES2Widget*  m_viewport;
    vp3DGeoGraph*   m_graph;
    bool            m_graph_populated;

    QHBoxLayout*    m_ly_main;
    QVBoxLayout     m_ly_vp_btns;
    QHBoxLayout     m_ly_btns;

    QSpacerItem* m_spacer0;
    QSpacerItem* m_spacer1;
    QLabel      m_label0;
    QLabel      m_label1;
    QPushButton m_btn0;
    QPushButton m_btn1;
    QPushButton m_btn2;
    QPushButton m_btn3;
    QPushButton m_btn4;
    QPushButton m_btn5;

    vpDataContainer* m_dataSource;
    QListWidget* m_fields_list;
    QVector<QListWidgetItem*> m_fields_items;
//
    void populate_listbox(QVector<QString> fields);
    void init_graph(vpDataContainer& datacontainer);
    void append_data_to_graph(vpData& datarow);

    
signals:
    
public slots:
    void adquire_data(vpDataContainer &datacontainer);
    void change_field_selection(QListWidgetItem* item);
};

#endif // VPGRAPHWIDGET_H
