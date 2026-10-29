#include "odbiornikcom.h"

OdbiornikCom::OdbiornikCom(QObject *parent)
    : QObject{parent}
{
    serialPort = new QSerialPort;
    serialPort->setBaudRate(115200);
    connect(serialPort, &QSerialPort::readyRead, this, &OdbiornikCom::parseComData);

}

bool OdbiornikCom::openCOM(const QString &portname){
    serialPort->setPortName(portname);
    if(serialPort->isOpen()){
        serialPort->close();
    }
    if(serialPort->open(QIODevice::ReadOnly)){
        return true;
    }else{
        return false;
    }
}

void OdbiornikCom::parseComData(){
    daneCOM.append(serialPort->readAll());
    while (daneCOM.size() >= sizeof(packet)) {
        uint8_t magic1 = static_cast<uint8_t>(daneCOM.at(0));
        uint8_t magic2 = static_cast<uint8_t>(daneCOM.at(1));
        if(magic1 == 0xAA && magic2 == 0xBB){
            packet odebranyPakiet;
            memcpy(&odebranyPakiet, daneCOM.constData(), sizeof(packet));
            uint8_t wyliczonyChecksum = checksum((uint8_t*)&odebranyPakiet.payload, sizeof(TELEMETRY));

            if (wyliczonyChecksum == odebranyPakiet.crc) {
                daneCOM.remove(0, sizeof(packet));
                emit parsedData(odebranyPakiet.payload);
            } else {
                daneCOM.remove(0, 1);
            }
        }
    }
}
