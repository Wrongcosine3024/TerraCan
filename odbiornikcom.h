#ifndef ODBIORNIKCOM_H
#define ODBIORNIKCOM_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include "tctelemetry.h"

class OdbiornikCom : public QObject
{
    Q_OBJECT
public:
    explicit OdbiornikCom(QObject *parent = nullptr);

    //x
    QSerialPort *serialPort;

    //fx
    void parseComData();

public slots:
    void podlaczCom(const QString &portname);
    void rozlaczCom();
private:
    QByteArray daneCOM;
    QString podlaczonyCOM;
signals:
    void parsedData(TELEMETRY telemetria);
    void connResult(bool result, QString result_string);
};

#endif // ODBIORNIKCOM_H
