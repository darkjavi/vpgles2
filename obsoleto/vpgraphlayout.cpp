#include "vpgraphlayout.h"

vpGraphLayout::vpGraphLayout(QWidget *parent) :
    QFrame(parent)
{
    m_ly_main = new QVBoxLayout(this);
    m_ly_main->addLayout(&m_ly_controls,1);
    m_ly_main->addLayout(&m_ly_vps,10);

    m_ly_main->setSpacing(0);
    m_ly_controls.setSpacing(0);
    m_ly_vps.setSpacing(0);
    this->setContentsMargins(QMargins(0,0,0,0));
    m_ly_main->setContentsMargins(QMargins(0,0,0,0));
    m_spacer_btns = new QSpacerItem( 0, 0, QSizePolicy::Expanding,
                                     QSizePolicy::Maximum);
    m_ly_controls.addItem(m_spacer_btns);
    m_label_controls.setText("Viewports:");
    m_ly_controls.addWidget(&m_label_controls);
    m_btn_add_vp.setText("+");
    m_btn_add_vp.setMaximumSize(25,25);
    m_ly_controls.addWidget(&m_btn_add_vp);
    m_btn_del_vp.setText("-");
    m_btn_del_vp.setMaximumSize(25,25);
    m_ly_controls.addWidget(&m_btn_del_vp);

    this->setFrameStyle(1);

    for ( ushort i = 0 ; i < DEFAULT_GRAPHS ; i++ )
    {
        vpGraphWidget* n_graph = new vpGraphWidget();
        m_graphs.append(n_graph);
        m_ly_vps.addWidget(m_graphs[i]);
        connect(this,SIGNAL(new_data(vpDataContainer&)),m_graphs[i],SLOT(adquire_data(vpDataContainer&)));
    }

}

vpGraphLayout::~vpGraphLayout()
{
    for ( ushort i = 0 ; i < m_graphs.size() ; i++ )
    {
        delete m_graphs[i];
    }
    delete m_spacer_btns;
    m_graphs.clear();
}

void vpGraphLayout::adquire_data(vpDataContainer &datacontainer)
{
    emit new_data(datacontainer);
}

