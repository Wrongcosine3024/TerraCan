#include "odbiornikcom.h"

OdbiornikCom::OdbiornikCom(QObject *parent)
    : QObject{parent}
{
    serialPort = new QSerialPort(this);
    serialPort->setBaudRate(115200);
    connect(serialPort, &QSerialPort::readyRead, this, &OdbiornikCom::parseComData);    //podpiecie sygnalu z portu com do funkcji parsujacej dane

}

void OdbiornikCom::podlaczCom(const QString &portname){    //funkcja otwierajaca polaczenie z portem COM
    if(!portname.isNull()){
        serialPort->setPortName(portname);
        if(serialPort->isOpen()){   // zamykamy jesli port jest juz otwatry (inaczej aplikacja sie scrashuje)
            serialPort->close();
        }
        if(serialPort->open(QIODevice::ReadOnly)){
            podlaczonyCOM = portname;
            emit connResult(true, "Sukces, połączono z portem: " + portname);
        }else{
            emit connResult(false, "Błąd połączenia: " + serialPort->errorString());
        }
    } else {
        emit connResult(false, "Wybierz port COM!");
    }
}

void OdbiornikCom::rozlaczCom(){
    serialPort->close();
    emit connResult(false, "Rozłączono z portem: " + podlaczonyCOM);
}

void OdbiornikCom::parseComData(){              //funkcja przetwarzajaca "szum" z portu COM
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
                continue;
            } else {
                daneCOM.remove(0, 1);
            }
        }
        daneCOM.remove(0, 1);
    }
}
