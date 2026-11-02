#ifndef PHYSICSHANDLER_H
#define PHYSICSHANDLER_H

#include <qobject.h>
#include <qthread.h>
#include "physicsengine.h"

class PhysicsHandler : public QObject
{
    Q_OBJECT
public:
    explicit PhysicsHandler(QObject *parent = nullptr, startData wejscie = {0,0,0,0,0,0,0.1,0});
    ~PhysicsHandler();
    PhysicsEngine *engine = nullptr;
    QThread *watek = nullptr;
    void zatrzymaj();

public slots:

signals:
    void obliczone(std::vector<Output> wyniki, bool czyZnalezionoPLadowania);
    void policz();
    void krokPoliczony(QString line);
};

#endif // PHYSICSHANDLER_H
