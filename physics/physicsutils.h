#ifndef PHYSICSUTILS_H
#define PHYSICSUTILS_H

struct startData{
    float pos_x;
    float pos_y;
    float pos_z;
    float vel_x;
    float vel_y;
    float vel_z;
    float dt;
    float timeout;
};

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

struct Output{
    Vec3 pos;
    Vec3 vel;
    Vec3 acc;
    Vec2 rot;
    float sim_time;
};

#endif // PHYSICSUTILS_H
