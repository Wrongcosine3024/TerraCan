#include "odbiornikcom.h"
#include <QObject>
#include <qthread.h>

#ifndef ODBIORNIKCOM_WORKER_H
#define ODBIORNIKCOM_WORKER_H

class OdbiornikWorker : public QObject{
    Q_OBJECT
private:
    QThread *watek;
    OdbiornikCom *radio;
public:
    explicit OdbiornikWorker(QObject *parent);
    ~OdbiornikWorker();
    void podlaczCom(const QString &portname);
    void rozlaczCom();

signals:
    void newData(TELEMETRY telemetria);
    void spodlaczCom(const QString &portname);
    void connResult(bool result, QString result_string);
    void srozlaczCom();
};

#endif // ODBIORNIKCOM_WORKER_H
