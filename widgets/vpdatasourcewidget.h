#ifndef VPDATASOURCEWIDGET_H
#define VPDATASOURCEWIDGET_H

#include <QFrame>
#include "vpdatasource.h"

namespace Ui {
class vpDataSourceWidget;
}

class vpDataSourceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit vpDataSourceWidget(QWidget *parent = 0);
    ~vpDataSourceWidget();
    void setDataSource(vpDataSource* ds);
private:
    Ui::vpDataSourceWidget *ui;
    vpDataSource*   m_dataSource;
private slots:
    void on_btn_open_clicked();
    void on_btn_exportCSV_clicked();
};

#endif // VPDATASOURCEWIDGET_H
