#ifndef WORKER_H
#define WORKER_H

#include <qobject.h>
#endif // WORKER_H

class worker : QObject{
    Q_OBJECT
public slots:
    void doWork(){

        emit resultReady();
    }

signals:
    void resultReady();
};
