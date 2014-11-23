#ifndef VPDATASETWIDGET_H
#define VPDATASETWIDGET_H

#include <QFrame>
#include "vpdataset.h"

namespace Ui {
class vpDataSetWidget;
}

class vpDataSetWidget : public QFrame
{
    Q_OBJECT
    
public:
    explicit vpDataSetWidget(vpDataSet* dataset, QWidget *parent = 0);
             vpDataSetWidget(QWidget *parent = 0);
    ~vpDataSetWidget();
    void setDataSet(vpDataSet* d);
private:
    Ui::vpDataSetWidget *ui;
    vpDataSet* m_dataset;
protected slots:
    void on_edit_id_returnPressed();
    void on_edit_id_editingFinished();
    void on_btn_delete_clicked();
    void datasetDeleted();
    void datasetUpdated();
};

#endif // VPDATASETWIDGET_H
