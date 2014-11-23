#ifndef VPDATAPOOLWIDGET_H
#define VPDATAPOOLWIDGET_H

#include <QGroupBox>
#include "vpdatapool.h"
#include "vpdatasetwidget.h"

namespace Ui {
class vpDataPoolWidget;
}

class vpDataPoolWidget : public QGroupBox
{
    Q_OBJECT
    
public:
    explicit vpDataPoolWidget(vpDataPool* dataPool, QWidget* parent = 0);
    vpDataPoolWidget(QWidget* parent = 0);
    ~vpDataPoolWidget();
    vpDataPool* dataPool()   {return m_dataPool;}
private:
    Ui::vpDataPoolWidget *ui;
    vpDataPool* m_dataPool;
    bool        m_shared_dataPool;
signals:

private slots:
    //void on_btn_addDataSet_clicked();
    void populateList();
    void on_btn_importCSV_clicked();
    void on_btn_importNMEA_clicked();
    void on_btn_importTorque_clicked();

    void on_btn_recursiveImportCSV_clicked();
    void on_btn_recursiveImportNMEA_clicked();
    void on_btn_recursiveImportTorque_clicked();

    void on_btn_connectGpsd_clicked();
    void on_btn_connectMF_clicked();
};
#endif // VPDATAPOOLWIDGET_H
