#include "vpdatawidgettable.h"
#include "ui_vpdatawidget.h"

vpDataWidgetTable::vpDataWidgetTable(vpDataPool *datapool, QWidget *parent) :
    vpDataWidget(datapool,parent)
{
    this->setWindowTitle("Table");
    this->addView(&m_table);
    m_table.setFont(QFont("ARIAL",8));
    refresh();
}

void vpDataWidgetTable::refresh()
{
    qDebug()<<"[DEBUG]->vpDataWidgetTable::refresh()-> Refresh: selected fields:"<<m_selectedYFields.count()<<"datasets:"<<m_selectedDatasets.count();
    m_table.clearContents();
    m_table.setColumnCount(m_selectedYFields.count());
    m_table.setRowCount(0);
    for(int f = 0 ; f < m_selectedYFields.count() ; f++)
    {
        QString field = m_selectedYFields[f];
        m_table.setHorizontalHeaderItem(f,new QTableWidgetItem(field));
        int populatedRows=0;
        for(int d = 0 ; d < m_selectedDatasets.count() ; d++)
        {
            vpDataSet* dataset = m_selectedDatasets[d];
            if(f==0)m_table.setRowCount(m_table.rowCount()+dataset->getRowCount());//ampliamos el tamaño de la tabla solo mientras recorremos el primer campo
            int fieldindex = dataset->indexOf(field);
            if(fieldindex < 0)
            {//si el dataset no tiene el campo se meten datos de relleno;
                for(int c = 0 ; c < dataset->getRowCount() ; c++)
                {
                    m_table.setItem(populatedRows,f,new QTableWidgetItem("nd"));
                    populatedRows++;
                }
            }
            else
            {
                for(int c = 0 ; c < dataset->getRowCount() ; c++)
                {
                    m_table.setItem(populatedRows,f,new QTableWidgetItem(dataset->getPosString(c,fieldindex)));
                    populatedRows++;
                }
            }
        }
    }
}
