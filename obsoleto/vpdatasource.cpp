#include "vpdatasource.h"

vpDataSource::vpDataSource(bool graphical_datasource, QWidget *parent) :
    QObject(parent)
{
    if (graphical_datasource)
    {
        m_widget = new vpDataSourceWidget(this);
    }
    else
    {
        m_widget = 0;
    }
}

vpDataSource::~vpDataSource()
{
    if(m_widget)
    {
        delete m_widget;
    }
    for ( int i = 0 ; i < m_parser_pool.count() ; i++ )
    {
        delete m_parser_pool[i];
        //sacar a punteros especificos y borrar con su destructor!!
    }
}

void vpDataSource::add_parser(vpParser *n_parser)
{
    m_parser_pool.append(n_parser);
    m_data_pool.append(&n_parser->dataset());
    if (m_widget)
    {
        m_widget->add_widget(&n_parser->widget());
    }
    emit data_pool_updated();
    //connect(n_parser,SIGNAL(destroyed()),this);
    //Si por lo que sea se borra el objeto hacer una funcion que lo saque de la lista
}

//SLOT
void vpDataSource::add_parserCSV(QString file)
{
    vpParserCSV* n_parser = new vpParserCSV;
    if (n_parser->parse_file(file))
    {
        add_parser(n_parser);
    }
    else
    {
        delete n_parser;
    }
}
