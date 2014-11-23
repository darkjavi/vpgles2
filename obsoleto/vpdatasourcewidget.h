#ifndef VPDATASOURCEWIDGET_H
#define VPDATASOURCEWIDGET_H

#include <QWidget>
#include "vpdatasource.h"


class vpDataSource;

namespace Ui {
class vpDataSourceWidget;
}

class vpDataSourceWidget : public QWidget
{
    Q_OBJECT
    
public:

    explicit vpDataSourceWidget(vpDataSource *parent);

    ~vpDataSourceWidget();
    void add_widget(QWidget* widget);
    
private:
    Ui::vpDataSourceWidget *ui;
    vpDataSource* m_dataSource;
private slots:
    void on_btn_addFile_clicked();
};

#endif // VPDATASOURCEWIDGET_H
