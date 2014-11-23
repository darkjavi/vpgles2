#include "vpparsermindflex.h"
#include <QtEndian>
#include <QDateTime>

vpParserMindFlex::vpParserMindFlex(QString source, btHelper *bthelper, QObject *parent) :
    vpParserSerialBluetooth(source,bthelper,parent)
{
    m_type = vpParser::ParserMindflexLive;
    m_readRaw =true;
}

 vpParserMindFlex::~vpParserMindFlex()
{

}

bool vpParserMindFlex::openSource(QString source)
{
    m_helper->pairDevice(source,1234);
    setBaudRate(57600);
    if(vpParserSerialBluetooth::openSource(source))
    {
        m_fields        << "Timestamp" << "Noise" << "Attention" << "Meditation" << "Delta" << "Theta"
                        <<  "LowAlpha" << "HighAlpha" << "LowBeta" << "HighBeta" << "LowGamma" << "MidGamma";
        m_auxFields     <<"TimeStamp"<<"RawValue";
        setTimeout(5000);
        return true;
    }
    return false;
}

bool vpParserMindFlex::checksum_is_valid(QByteArray& data)
{
    //qDebug() << "[WARN]mfParser::check_data-->comprobando paquete:"<< data.size();
    uint index = 0;
    if ( !(( data[index++] == NS_SYNC_CODE ) && ( data[index++] == NS_SYNC_CODE )) )
    {
        qDebug() << "[WARN]mfParser::check_data--> La cadena no empieza por los los valores de sync";
        return false;
    }
    ushort payload_size = data[index++];
    //qDebug() << "[INFO]mvpParserMindFlex::checksum_is_valid--> Payload size:"<< payload_size;
    if ( ! (payload_size < 170) )
    {
        qDebug() << "[WARN]vpParserMindFlex::checksum_is_valid--> tamaño de payload fuera de rango" << payload_size << "/" << NS_SYNC_CODE ;
        return false;
    }
    char payload_checksum = 0X00;
    for ( uint i = 0; i < payload_size ; i++)
    {
        payload_checksum += data[index++];
    }
    payload_checksum = ~payload_checksum & 0xFF;
    //qDebug() << "[INFO]mfParser::checksum_is_valid->" << payload_checksum << "/" << data[index];
    if ( payload_checksum != data[index] )
    {
        qDebug() << "[WARN]vpParserMindFlex::checksum_is_valid-> checksum incorrecto!!"<< (quint8)payload_checksum << "/" << (quint8)data[index];
        return false;
    }
    return true;
}

QVector<double> vpParserMindFlex::parse_rawPacket(QByteArray &data)
{
    double timestamp=QDateTime::currentDateTime().currentMSecsSinceEpoch();
    QVector<double> result;
    result.append(timestamp);
    if(data.count()>8)
    {
        qDebug()<<"[DEBUG]vpParserMindFlex::parse_rawPacket-> EXTRA FIELDS!!!!"<< data.count()-7 << (quint8) data[8];
    }
    qint16 raw = 0;
    raw = (data[4] << 8) | data[5];
    result.append(qFromBigEndian(raw));
    return result;
}

QVector<double> vpParserMindFlex::parse_eegPacket(QByteArray& data)
{
    double timestamp=QDateTime::currentDateTime().currentMSecsSinceEpoch();
    ushort payload_size = data[2];
    quint8 poor_signal = 199;
    quint8 attention = 0;
    quint8 meditation = 0;
    ulong eeg_power[NS_POWER_BANDS];

    for ( quint8 index = 3 ; index < (payload_size +3) ; index++ )
    {
        if  ( data[index] == NS_POOR_SIGNAL)
        {
            poor_signal = data[++index];
        }
        else if ( data[index] == NS_ATTENTION)
        {
            attention = data[++index];
        }
        else if ( data[index] == NS_MEDITATION)
        {
            meditation = data[++index];
        }
        else if ( data[index] == NS_ASIC_EEG_POWER)
        {
            quint8 length = data[++index];
            if (length != (NS_POWER_BANDS*3))
            {
                qDebug() << "[WARN]mfParser::parse_packet->paquete NS_ASIC_EEG_POWER no tiene los campos esperados:" << length << "/"<< NS_POWER_BANDS*3;
                index += length;
            } else
            {
                for (quint8 b = 0 ; b < NS_POWER_BANDS ; b++)
                {
                    quint32 tmp_int =(quint32) (0x00111111 & ( (data[index+1] << 16) | (data[index+2] << 8) | (data[index+3])) );
                    eeg_power[b] = qFromBigEndian(tmp_int);
                    index+=3;
                }
            }
        }
        else if(data[index] == NS_RAW_WAVE)
        {
            qDebug() << "[DEBUG]mfParser::parse_packet-> RAW WAVE DATA!!!!";//no deberia de llegar aqui porque se esta filtrando antes
            index++;
            index++;
            index++;
        }
        else
        {
            qDebug() << "[WARN]mfParser::parse_packet-> Campo desconocido" << index << ":" << (quint8)data[index] ;
            index++;
        }

    }
    QVector<double> n_data;
    n_data.append(timestamp/1000.0f);
    if (poor_signal > 200)
        poor_signal = 201;

    n_data.append(poor_signal);
    n_data.append(attention);
    n_data.append(meditation);
    for( int i = 0 ; i < NS_POWER_BANDS ; i++ )
    {
        n_data.append(eeg_power[i]);
    }
    return n_data;
}

bool vpParserMindFlex::parse_rxBuffer()
{
    //qDebug()<<"vpParserMindFlex::parse_rxBuffer-> data on buffer:";
    //QString str;
    //for(int i = 0 ; i<m_rxBuffer.count();i++)
    //{
    //    str.append("[").append(QString::number((quint8)m_rxBuffer[i])).append("]");
    //}
    //qDebug()<<str;        
    bool packet_parsed = false;
    ushort sync_index = 0;
    ushort last_packet_end = 0;
    QByteArray tmp_buffer;
    for ( ushort i = 0 ; i < m_rxBuffer.count()-1 ; i++ )
    {
        if (( m_rxBuffer[i] == NS_SYNC_CODE ) && ( m_rxBuffer[i+1] == NS_SYNC_CODE ))
        {
            sync_index = i;
            tmp_buffer.clear();
            //qDebug() << "Sync found :" << sync_index;
            i += 2;//avanzamos hasta el tercer byte del paquete
            if ( m_rxBuffer.count() > i ) // Nos aseguramos que no vamos a intentar leer fuera del array
            {
                quint8 payload_size = m_rxBuffer[i];
                //qDebug() << "Payload size:" << payload_size;
                quint8 packet_size =  payload_size +4;//payload + 2 sync bytes + 1 size byte + 1 checksum byte
                if ( m_rxBuffer.count() >= sync_index + packet_size ) //Comprobamos si estan el el buffer todos los datos del paquete
                {
                    i += payload_size + 2;
                    tmp_buffer=m_rxBuffer.mid(sync_index,packet_size);
                    if ( checksum_is_valid(tmp_buffer) )
                    {
                        if(tmp_buffer[3]==NS_RAW_WAVE)
                        {
                            if(m_readRaw)
                            {
                                emit newAuxData(parse_rawPacket(tmp_buffer));
                            }
                        }
                        else
                        {
                            emit newData(parse_eegPacket(tmp_buffer));
                        }
                       last_packet_end = i;
                       packet_parsed = true;
                    }
                }
            }
        }
    }
    if (packet_parsed)
    {
        //qDebug()<< "ajustando el buffer al ultimo paquete parseado" << last_packet_end;
        QByteArray tmp_buffer2 = m_rxBuffer.mid(last_packet_end);
        m_rxBuffer.clear();
        m_rxBuffer.append(tmp_buffer2);
    }
    else if (sync_index > 0)
    {
        qDebug()<< "[WARN]vpParserMindFlex::parse_rxBuffer->Parece que hay basurilla en el buffer, cortandolo hasta la posicion" << sync_index ;
        QByteArray tmp_buffer2 = m_rxBuffer.mid(sync_index);
        m_rxBuffer.clear();
        m_rxBuffer.append(tmp_buffer2);
    }
    else if(m_rxBuffer.count()>100)
    {
        qDebug()<<"[WTF]vpParserMindFlex::parse_rxBuffer-> FUCKING HELL! buffer is plenty of f**k*ng garbage, ARRRRRRRGGGRGGRGRG";
        m_rxBuffer.clear();
        emit error();
        resetSerial();
    }
    return packet_parsed;
}
