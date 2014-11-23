#include "vpparsercsv.h"
#include <QStringList>

vpParserCSV::vpParserCSV(QString source, QObject *parent) :
    vpParser(source,parent)
{
    setSeparators(",","\n");
    m_dataInStrings = true;
}

bool vpParserCSV::openSource(QString source)
{
    m_source = source;
    QFile f(source);
    if(!f.open(QIODevice::ReadOnly))
    {
        return false;
    }
    f.close();
    m_fields = readFields(source,m_fieldSep,m_lineSep);
    emit dataRx();
    return true;
}

QVector<QString>    vpParserCSV::readFields(QString filename, QString fieldSep, QString lineSep)
{
    QVector<QString> result;
    QFile f(filename);
    if(!f.exists())                     return result;
    if(!f.open(QIODevice::ReadOnly))    return result;
    if(lineSep == "\n")
    {
        //if(!f.canReadLine()) return result;
        QString dataline = f.readLine();
        QVector<QString> raw_fields = dataline.split(fieldSep).toVector();
        for(int i = 0 ; i < raw_fields.count() ; i++)
        {
            result.append(raw_fields[i].trimmed());
        }
    }
    else
    {
        QString dataline = f.read(1024);
        QVector<QString> raw_fields = dataline.split(fieldSep).toVector();
        for(int i = 0 ; i < raw_fields.count() ; i++)
        {
            result.append(raw_fields[i].trimmed());
        }
    }
    return result;
}

QVector<QVector<QString> >  vpParserCSV::readStrings(int start, int count)
{
    QVector<QVector<QString> > result;
    QFile f(m_source);
    if(!f.exists())                     return result;
    if(!f.open(QIODevice::ReadOnly))    return result;
    if(m_lineSep == "\n")
    {
        f.readLine();//Saltamos la linea de cabecera
        for (int s = 0 ; s < start ; s++)
            f.readLine();
        if(!f.canReadLine())
        {
            qDebug() << "Fuera de rango";
        }
        int readed=0;
        while (f.canReadLine())
        {
            QString rawrow = f.readLine();
            QVector<QString> row = rawrow.split(m_fieldSep).toVector();
            result.append(row);
            readed++;
            if(count && count >= readed) break;
        }
        if(count && readed < count)
        {
            qDebug() << "Se ha pedido mas de lo que habia!!";
        }
    }
    else
    {
        qDebug()<< "NOT IMPLEMENTED (YET!)";
    }

//    if(start==0&&count==0)//Carga completa del fichero
//    {
//        QString raw_data = f.readAll();
//        f.close();
//        QStringList data_lines = raw_data.split(m_lineSep);
//        raw_data.clear();
//        quint32 datacount=data_lines.count();
//        for(uint i = 1 ; i < datacount ; i++)
//        {
//            //en otro rato estaria bien hacer algunas comprobaciones para comprobar que todo tiene sentido
//            //pe mismo numero de columnas en todas las filas y numero de campos
//            QVector<QString> row = data_lines[i].split(m_fieldSep).toVector();
//            result.append(row);
//            if(i%10==0)
//            {
//                emit progress(i*100/datacount);
//            }
//        }
//    }
//    {//carga selectiva (start y count)
//        //implementame.......
//    }
    return result;
}

void vpParserCSV::closeSource()
{//no hay mucho que hacer pues ya esta todo cerrado....

}
