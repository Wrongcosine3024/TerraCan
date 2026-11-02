#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <qobject.h>
#include <math.h>
#include <qsettings.h>
#include <vector>
#include "physicsutils.h"

float isa_air_density(float H);

class PhysicsEngine : public QObject
{
    Q_OBJECT
public:
    explicit PhysicsEngine(QObject *parent = nullptr, startData dane_uzytkownika = {0,0,0,0,0,0,0.1,0});
    CanSat TerraCan;
    float chute_delay;
    float x_start;
    float y_start;
    float z_start;
    float vx_start;
    float vy_start;
    float vz_start;
    float dt;
    int timeout; // W KROKACH - nie sekundach: timeout =  żądany czas czas/liczba krokow
    unsigned int dzielnik;
    void zatrzymaj();   //funkcja przerywajaca petle obliczen
public slots:
    std::vector<float> derive();
private:
    QSettings ustawienia;
    const float g = 9.81;
    const float pi = 3.14159;
    bool running = true;    //zmienna pomocnicza sluzaca do przerwania petli obliczen
signals:
    void skonczylem();
    void krokPoliczony(QString line);
};

#endif // PHYSICSENGINE_H
