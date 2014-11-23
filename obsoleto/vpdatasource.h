#ifndef VPDATASOURCE_H
#define VPDATASOURCE_H

#include <QThread>
#include <QVector>
#include <QLayout>

#include "vpparserincludes.h"
#include "vpdatasourcewidget.h"
#include "vpwizardmindflex.h"

class vpDataSourceWidget;

class vpDataSource : public QObject
{
    Q_OBJECT
public:
    explicit vpDataSource(bool graphical_datasource = true,QWidget *parent = 0);
    ~vpDataSource();

    QVector<vpParser*>    parser_pool()   {return m_parser_pool;}
    QVector<vpDataSet*>   data_pool()     {return m_data_pool;}
    vpDataSourceWidget*   widget()        {return m_widget;}

protected:
    void add_parser(vpParser* n_parser);

    QVector<vpParser*>  m_parser_pool;
    QVector<vpDataSet*> m_data_pool;

    vpDataSourceWidget* m_widget;
signals:
    void data_pool_updated();

public slots:
    void add_parserCSV(QString file);
};

#endif // VPDATASOURCE_H
