#include "physicshandler.h"

PhysicsHandler::PhysicsHandler(QObject *parent, startData wejscie)
    : QObject{parent}
{
    engine = new PhysicsEngine(nullptr, wejscie);
    watek = new QThread(this);

    engine->moveToThread(watek);

    connect(watek, &QThread::finished, engine, &QObject::deleteLater);
    connect(watek, &QThread::finished, watek, &QObject::deleteLater);
    connect(watek, &QThread::started, engine, &PhysicsEngine::derive);
    connect(engine, &PhysicsEngine::skonczylem, this, &PhysicsHandler::obliczone);
    connect(engine, &PhysicsEngine::krokPoliczony, this, &PhysicsHandler::krokPoliczony);
    //connect(this, &PhysicsHandler::policz, engine, &PhysicsEngine::derive);

    watek->start();
}

PhysicsHandler::~PhysicsHandler(){
    watek->quit();
    watek->wait();
    watek->deleteLater();
}

void PhysicsHandler::zatrzymaj(){
    engine->zatrzymaj();
}