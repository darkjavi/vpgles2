#ifndef VPPARSERMINDFLEX_H
#define VPPARSERMINDFLEX_H

#include <QProcess>
#include "vpparserserialbluetooth.h"
#include "vpdatamindflex.h"

#define NS_POOR_SIGNAL      (char)0x02
#define NS_ATTENTION        (char)0x04
#define NS_MEDITATION       (char)0x05
#define NS_BLINK_STRENGHT   (char)0x16
#define NS_RAW_WAVE         (char)0x80
#define NS_ASIC_EEG_POWER   (char)0x83
#define NS_SYNC_CODE        (char)0xAA

#define NS_MODE_9600_NORMAL (char)0x00
#define NS_MODE_1200_NORMAL (char)0x01
#define NS_MODE_576K_RAW    (char)0x02

#define NS_POWER_BANDS      8
class vpDataSet;
class vpParserMindFlex : public vpParserSerialBluetooth
{
    Q_OBJECT
public:
    explicit vpParserMindFlex(QString source = "none", btHelper* bthelper=0, QObject *parent = 0);
    ~vpParserMindFlex();
    bool openSource(QString source);
protected:
    bool                   m_readRaw;
    bool                   parse_rxBuffer();
    static QVector<double> parse_eegPacket(QByteArray& data);
    static QVector<double> parse_rawPacket(QByteArray& data);
    static bool            checksum_is_valid(QByteArray& data);
signals:
    void rawDataUpdated();

public slots:

};

#endif // VPPARSERMINDFLEX_H
