#ifndef VPDATASET_H
#define VPDATASET_H

#include <QObject>
#include <QDateTime>

#include "vpparsercsv.h"
#include "vpparsermindflex.h"
#include "vpparsernmea.h"
#include "vpdatagpstrack.h"
#include "vpdatamindflex.h"

class vpDataSet : public QObject
{
    Q_OBJECT
public:
    explicit vpDataSet(QObject *parent = 0);
            ~vpDataSet();
    QString     id(){return m_data->id();}
    vpParser*   parser(){return m_source;}
    vpData*     data()  {return m_data;}
    void        setBtHelper(btHelper* h){m_btHelper= h;}
    bool        isValid();
    bool        hasParser();
protected:
    vpParser*   m_source;
    vpData*     m_data;
    btHelper*   m_btHelper;
    void setParser(vpParser* p);
signals:
    void dataUpdated();
    void incompleteFields();
    void parserDied();
protected slots:
    void parserDeleted();
public slots:
    void setID(QString id)            {m_data->setID(id);}
    void clear();
    void exportCSV(QString filename)
    {
        if(m_source)
        {
            m_source->exportCSV(filename,true);
        }
        else
        {
            vpParser::exportCSV(filename,*m_data,true);
        }
    }
    void importCSV(QString filename);
    void importNMEA(QString filename);
    void importTorqueLog(QString filename);
    void importMindflexCSV(QString filename);
    void connectMindflex(QString macaddr);
    void connectGpsd(QString host = "localhost",int port =2947);
};

#endif // VPDATASET_H
