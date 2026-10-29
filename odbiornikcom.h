#ifndef ODBIORNIKCOM_H
#define ODBIORNIKCOM_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <vector>
#include "tctelemetry.h"

class OdbiornikCom : public QObject
{
    Q_OBJECT
public:
    explicit OdbiornikCom(QObject *parent = nullptr);

    //x
    QSerialPort *serialPort;
    std::vector<QString> avalaiblePorts;
    unsigned int index;

    //fx
    bool openCOM(const QString &portname);
    void closeCOM();
    void parseComData();

private:
    QByteArray daneCOM;
signals:
    void parsedData(TELEMETRY telemetria);
};

#endif // ODBIORNIKCOM_H
