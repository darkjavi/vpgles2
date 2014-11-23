#ifndef VPGRAPHLAYOUT_H
#define VPGRAPHLAYOUT_H

#include <QWidget>

#define DEFAULT_GRAPHS  2

#include "vpgraphwidget.h"
class vpGraphLayout : public QFrame
{
    Q_OBJECT
public:
    explicit vpGraphLayout(QWidget *parent = 0);
    ~vpGraphLayout();
private:
    QVBoxLayout *m_ly_main;
    QHBoxLayout m_ly_controls;
    QVBoxLayout m_ly_vps;

    QLabel      m_label_controls;
    QPushButton m_btn_add_vp;
    QPushButton m_btn_del_vp;
    QSpacerItem* m_spacer_btns;
    QVector<vpGraphWidget*> m_graphs;
signals:
    void new_data(vpDataContainer& datacontainer);

public slots:
    void adquire_data(vpDataContainer& datacontainer);

};

#endif // VPGRAPHLAYOUT_H
