#ifndef PHYSICSUTILS_H
#define PHYSICSUTILS_H

struct Vec3{
    float x,y,z;
};

struct Vec2{
    float x,y;
};

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

#endif // PHYSICSUTILS_H
