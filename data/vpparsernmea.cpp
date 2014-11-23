#include "vpparsernmea.h"
#include <QDateTime>

vpParserNMEA::vpParserNMEA(QString source, QObject *parent) :
    vpParser(source,parent)
{
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(readGpsdSocket()));
    m_fields << "Timestamp" << "Latitude" << "Longitude" << "Altitude" << "Speed" << "Bearing" << "Fix" << "Satellites";
}

bool vpParserNMEA::openSource(QString source)
{
//init fields
    m_source = source;
    if(source.startsWith("GPSD"))
    {
        qDebug() << "vpParserNMEA::openSource-> GPSD:" << source;
        QStringList args = source.split(":");
        if(args.count() != 3)return false;
        connectGpsd(args[1],args[2].toInt());
        return false;
    }
    qDebug() << "vpParserNMEA::openSource-> Abriendo:" << source;
    QFile f(m_source);
    if(!f.open(QIODevice::ReadOnly)) return false;
//    int c = 0;
//    qDebug() << f.canReadLine();
//    while (f.canReadLine())
//    {
//        m_buffer.append(f.readLine());
//        qDebug()<< c++;
//        if(c>20)
//        {
//            c=0;
//            parseBuffer();
//        }
//    }
    m_buffer.append(f.readAll());
    parseBuffer();
    m_buffer.clear();
    return true;
}

void vpParserNMEA::closeSource()
{

}

bool vpParserNMEA::isOpen()
{
    return m_socket.isOpen();//???
}

bool vpParserNMEA::startGpsd()
{
    stopGpsd();
    QStringList devices = getGpsCandidates();
    if(devices.count()==0)
    {
        //emit debugMsg("[ERROR]cpcServiceGps::startService()->No devices :S");
        return false;
    }
    for(int i = 0 ; i < devices.count() ; i++)
    {
        //emit debugMsg("[INFO]cpcServiceGps::startService()->Probing device "+devices.at(i));
        m_gpsdProcess.start("gpsd -n -N -b "+devices.at(i));
        m_gpsdProcess.waitForStarted();
        Sleeper::sleep(1);
        if(isGpsdWorking("localhost",2947))
        {
            return true;
        }
        m_gpsdProcess.kill();
        m_gpsdProcess.waitForFinished();
    }
    //emit debugMsg("[ERROR]cpcServiceGps::startService()->No suitable devices....");
    //m_lastErrorString="No suitable devices";
    return false;
}

void vpParserNMEA::stopGpsd()
{
    m_gpsdProcess.kill();
    m_gpsdProcess.waitForFinished();
    QProcess p;
    p.start("killall gpsd");
    p.waitForFinished();
}

bool vpParserNMEA::isGpsdWorking(QString gpsdHost, int gpsdPort)
{
    QTcpSocket s;
    s.connectToHost(gpsdHost,gpsdPort);
    s.waitForConnected(5000);
    if(s.state()==QAbstractSocket::UnconnectedState) return false;
    QByteArray init_string = "?WATCH={\"nmea\":true};";
    s.write(init_string);
    for(int i = 0 ; i < 5 ; i++) //le damos 5 intentos para leer una sentencia gps
    {
        s.waitForReadyRead(1000);
        QString out = s.readAll();
        if(out.contains("$GPRMC"))
        {
            s.close();
            return true;
        }
    }
    s.close();
    return false;
}

QStringList vpParserNMEA::getGpsCandidates()
{
    QStringList candidates;
    candidates << "/dev/ttyUSB0" << "/dev/ttyUSB1"  << "/dev/ttyUSB2" << "/dev/ttyUSB3"<< "/dev/ttyUSB4" << "/dev/ttyUSB5";
    QStringList devices;
    for(int i = 0 ; i < candidates.count() ; i++)
    {
        if(QFile(candidates[i]).exists()) devices.append(candidates[i]);
    }
    return devices;
}

void vpParserNMEA::parseBuffer()
{
    QStringList lines = m_buffer.split("\n");
    if(lines.last().isEmpty()) lines.removeLast();
    uint last_parsed = 0;
    double timestamp = 0;
    double lat = 0;
    double lon = 0;
    double alt = 0;
    double speed = 0;
    double bearing = 0;
    double satellites = 0;
    double fix = 0;

    for (int l = 0 ; l < lines.count() ; l ++)
    {
        QString dataLine = lines[l];
        if(dataLine.startsWith("$GPRMC"))
        {
            //la sentencia RMC es la ultima antes de un nuevo punto gps

            //extract time
            QStringList fields = dataLine.split(",");
            if(fields.count() > 10) // si hay menos es que la linea esta corrupta
            {
                QString datestr = fields[9];
                datestr.insert(4,"20");          // hack valid till 2099!
                QString timestr = fields[1];
                QDateTime dt;
                if(datestr.count() == 6)        //looks like out of nowhere gpsd started to deliver time with 3 decimals
                {
                    dt = QDateTime(QDate::fromString(datestr,"ddMMyyyy"),QTime::fromString(timestr,"hhmmss"),Qt::UTC);
                }
                else
                {
                    dt = QDateTime(QDate::fromString(datestr,"ddMMyyyy"),QTime::fromString(timestr,"hhmmss'.'zzz"),Qt::UTC);
                }
                //qDebug() << "rawTime:"<<dt.toMSecsSinceEpoch();
                timestamp = dt.toMSecsSinceEpoch()/1000.0d;
                //qDebug() << "Time:"<<QString::number(timestamp,'f',2);
                //extrae el resto de campos de la sentencia rmc
                lat = fields.at(3).toDouble()/100.0d;
                if ( fields.at(4) == "S" )  lat *= -1;
                lon= fields.at(5).toDouble()/100.0d;
                if ( fields.at(6) == "W" ) lon *= -1.0d;
                bearing= fields.at(8).toDouble();
                speed = fields.at(7).toDouble() * 1.852d; //1knot = 1.852km/h
            }

            if( timestamp == 0 )
            {
                qDebug() << "[NMEAPARSER] Basurilla en el buffer...";
            } else {
                QVector<double> gpsdata;
                gpsdata << timestamp << lat << lon << alt << speed << bearing << fix <<satellites;
                emit newData(gpsdata);
            }
            timestamp = 0;
            lat = 0;
            lon = 0;
            alt = 0;
            speed = 0;
            bearing = 0;
            fix = 0;
            satellites = 0;

            last_parsed = l; //last parsed position
        } else if(dataLine.startsWith("$GPGGA")) {
            QStringList fields = dataLine.split(",");
            if(fields.count() > 10) // si hay menos es que la linea esta corrupta
            {
                alt = fields.at(9).toFloat();
                if (fields.at(10) == "K") {alt*=1000.0d;}
                satellites = fields.at(7).toDouble();
            }
            //double gpgga_lat = fields.at(2).toDouble()/100.0f;
            //if ( fields.at(3) == "S" ) {gpgga_lat *= -1;}
            //double gpgga_lon = fields.at(4).toDouble()/100.0f;
            //if ( fields.at(5) == "W" ) {gpgga_lon *= -1;}
            //double gpgga_hdop = fields.at(8).toDouble();
        } else if(dataLine.startsWith("$GPGSV")) {
            // Not needed!
            //QStringList fields = dataLine.split(",");
            //unsigned short gpgsv_sats_view = fields.at(3).toShort();
        } else if(dataLine.startsWith("$GPGSA")) {
            QStringList fields = dataLine.split(",");
            if(fields.count() > 2) // si hay menos es que la linea esta corrupta
            {
                fix=fields.at(2).toDouble();
            }
            //float gpgsa_vdop= fields.at(18).split("*").at(0).toFloat();
        }
    }
    if( last_parsed > 0)
    {
        int index = -1;
        index = m_buffer.indexOf(lines[last_parsed]);
        if(index <= 0)
        {
            qDebug() << "[WTF]vpParserNMEA::parseBuffer()-> Fuck! no se como limpiar el buffer";
        } else {
            //qDebug() << "nmea-parser: limpiando buffer";
            //qDebug() << "Ultima linea parseada:"<<lines[last_parsed];
            //qDebug() << "Ultima linea en el buffer" << lines.last();
            m_buffer = m_buffer.mid(index+lines[last_parsed].count());
            //qDebug() << "Resto en el buffer:" << m_buffer;
        }
    }
}

bool vpParserNMEA::connectGpsd(QString host, int port)
{
    if(m_socket.isOpen()) m_socket.close();
    m_gpsdHost = host;
    m_gpsdPort = port;
    if(!isGpsdWorking(host,port))
    {
        if ( (host != "localhost") || (!startGpsd()) ) return false;
    }
    m_socket.connectToHost(host,port);
    m_socket.waitForConnected();
    if(m_socket.state()==QAbstractSocket::UnconnectedState) return false;
    QByteArray init_string = "?WATCH={\"enable\":true,\"nmea\":true};";
    m_socket.write(init_string);
    return true;
}

void vpParserNMEA::readGpsdSocket()
{
    while(m_socket.canReadLine())
    {
        m_buffer.append(m_socket.readLine());
    }
    parseBuffer();
}
