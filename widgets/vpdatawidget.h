#ifndef VPDATAWIDGET_H
#define VPDATAWIDGET_H

#include <QFrame>
#include <QListWidgetItem>
#include <QTimer>

#include "vpdatapoolwidget.h"

namespace Ui {
class vpDataWidget;
}

class vpDataWidget : public QFrame
{
    Q_OBJECT
    
public:
    explicit vpDataWidget(vpDataPool* datapool,QWidget *parent = 0);
    ~vpDataWidget();
protected:
    vpDataPool*         m_dataPool;
    QVector<vpDataSet*> m_selectedDatasets;
    QVector<QString>    m_selectedYFields;
    QString             m_selectedXField;
    QString             m_selectedZField;
    QTimer              m_timer;
    int                 m_animationCounter;

    Ui::vpDataWidget *ui;
    bool m_autoSelection;
    void addView(QWidget* dataview);
protected slots:
    void populateUI();
    void dataPoolUpdated();
    void dataSetUpdated();
    void dataPoolDeleted();
    void dataSetDeleted(QObject*);
    void newDataset(vpDataSet*);
    void addDataSet(vpDataSet*);
    void removeDataSet(vpDataSet*);

    void on_combo_xField_currentIndexChanged(QString);
    void on_combo_zField_currentIndexChanged(QString);
    void on_list_yFields_itemChanged(QListWidgetItem*);
    void on_listDataSets_itemChanged(QListWidgetItem*);
    void on_btn_reset_clicked();
    void animateClose();
    void animateOpen();
    void mouseMoveEvent(QMouseEvent *e);
public slots:
    void clear();
    virtual void refresh(){qDebug("refresh() not implemented in vpDataWidget");}
};

#endif // VPDATAWIDGET_H
