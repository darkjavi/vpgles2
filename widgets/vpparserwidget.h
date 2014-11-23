#ifndef VPPARSERWIDGET_H
#define VPPARSERWIDGET_H

#include <QFrame>
#include "vpparser.h"

namespace Ui {
class vpParserWidget;
}

class vpParserWidget : public QFrame
{
    Q_OBJECT
    
public:
    explicit vpParserWidget(QWidget *parent = 0);
    ~vpParserWidget();
    void set_parser(vpParser *parser);
private:
    Ui::vpParserWidget *ui;
    vpParser* m_parser;
public slots:
    void open_exportDialog();
    void on_btn_info_clicked();
    void on_btn_delete_clicked();
    void on_btn_record_clicked(bool);
};

#endif // VPPARSERWIDGET_H
