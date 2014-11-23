#include "vpparsertorque.h"
#include <QStringList>

vpParserTorque::vpParserTorque(QString source, QObject *parent) :
    vpParserCSV(source,parent)
{
    m_type=vpParser::ParserTorqueCSV;
}

//void vpParserTorque::process()
//{
//    QString raw=m_sourceFile.readAll();
//    QStringList lines = raw.split("\n");
//    quint8 headers_found=0;
//    for( int l = 0;l<lines.count();l++)
//    {
//        QStringList fields=lines[l].split(",");
//        //Leemos la cabecera con los campos
//        if(fields[0]=="GPS Time")
//        {
//            qDebug()<<"[DEBUG]vpParserTorque::process()-> Header found,"<<fields.count()<<"fields";
//            QStringList clean_fields;
//            for(int i = 0;i<fields.count();i++)
//            {
//                clean_fields.append(fields.at(i).trimmed());
//            }
//            if(headers_found>0)
//            {
//                qDebug()<<"[WARN]vpParserTorque::process()->"<< headers_found << " extra headers in the same file!";
//                if(clean_fields.toVector()==m_dataSet->getFields())
//                {
//                    qDebug()<<"[WARN]vpParserTorque::process()-> same fields on new header, moving on...";
//                }
//                else
//                {
//                    qDebug()<<"[WARN]vpParserTorque::process()->Fields have changed!, STOPPING IMPORT!";
//                    return;
//                }
//            }
//            else
//            {
//                m_dataSet->initDataset(m_source.right(20),clean_fields.toVector());
//            }
//            headers_found++;
//        }
//        //leemos linea de datos
//        else
//        {
//            if(!headers_found)
//            {
//                qDebug()<<"[WARN]vpParserTorque::process()-> No headers!!";
//                return;
//            }
//            QVector<double> data;
////hacer excepcion para el tiempo!!!
//            for(int i=0;i<fields.count();i++)
//            {
//                data.append(fields.at(i).toDouble());
//            }
//            m_dataSet->appendRowDoubles(data);
//        }
//        quint8 p=l*100/lines.count();
//        if(p%5==0)
//        {
//            emit progress(p);
//        }
//    }
//}
