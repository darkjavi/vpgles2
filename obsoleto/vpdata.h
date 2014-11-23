#ifndef VPDATA_H
#define VPDATA_H

#include <QObject>
#include <QDebug>

class vpData : public QObject
{
    Q_OBJECT
public:
    explicit vpData(QObject *parent = 0);
    vpData(QVector<qreal> data_row, QObject *parent = 0);
    vpData(const vpData& obj);//constructor copia
    void operator =(const vpData& obj);

    void set_datarow(QVector<qreal> data_row);
    QVector<qreal>& datarow()   {return m_data_row;}

    bool is_valid()             {return m_valid;}
    QString info(bool to_screen);
protected:
    bool m_valid;
    QVector<qreal> m_data_row;
signals:
    
public slots:
    
};

#endif // VPDATA_H
