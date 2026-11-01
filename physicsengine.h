#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <qobject.h>
#include <vector>

struct Vec3{
    float x,y,z;
};

struct Vec2{
    float x,y;
};

double isa_air_density(double H);



struct CanSat
{
    Vec3 pos;
    Vec3 vel;
    double Cd;
    double A;
    double mass;
    Vec3 acc;
    Vec2 rot;  //rotacja wyliczana z f. tryg
    Vec3 F;    //sila dla kazdej z osi
};

class PhysicsEngine : public QObject
{
    Q_OBJECT
public:
    explicit PhysicsEngine(QObject *parent = nullptr);
    float x_start;
    float y_start;
    float z_start;
    float vx_start;
    float vy_start;
    float vz_start;
    float dt;
    int timeout;
private:
    const float g = 9.81;
    const float pi = 3.14159;


signals:
};

std::vector<float> derive(){
    vector<float>
}

#endif // PHYSICSENGINE_H
