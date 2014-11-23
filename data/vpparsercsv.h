#ifndef VPPARSERCSV_H
#define VPPARSERCSV_H

#include "vpparser.h"

class vpParserCSV : public vpParser
{
    Q_OBJECT
public:
    explicit    vpParserCSV(QString source = "none", QObject *parent = 0);
    bool        openSource(QString source);
    QVector<QVector<QString> >  readStrings(int start = 0, int count = 0);
    bool        isOpen(){return false;}
protected:
    QString m_fieldSep;
    QString m_lineSep;
    static QVector<QString>    readFields(QString filename, QString fieldSep=",", QString lineSep="\n");
signals:
    
public slots:
    void setSeparators(QString fieldSep = ",",QString lineSep="\n"){m_lineSep=lineSep;m_fieldSep=fieldSep;}
    void closeSource();
};

#endif // VPPARSERCSV_H
