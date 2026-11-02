#include "physicsengine.h"
#include <qthread.h>

PhysicsEngine::PhysicsEngine(QObject *parent, startData dane_uzytkownika)
    : QObject{parent}
{
    TerraCan.mass = ustawienia.value("TerraCan/mass", 0.32).toDouble();
    TerraCan.A = ustawienia.value("TerraCan/Cd", 0.0045).toDouble();
    TerraCan.Cd = ustawienia.value("TerraCan/opor", 0.8).toDouble();
    dzielnik = ustawienia.value("TerraCan/dzielnik", 100).toInt();
    chute_delay = ustawienia.value("TerraCan/chute", 0).toFloat();

    x_start = dane_uzytkownika.pos_x;
    y_start = dane_uzytkownika.pos_y;
    z_start = dane_uzytkownika.pos_z;
    vx_start = dane_uzytkownika.vel_x;
    vy_start = dane_uzytkownika.vel_y;
    vz_start = dane_uzytkownika.vel_z;
    dt = dane_uzytkownika.dt;
    timeout = dane_uzytkownika.timeout/dane_uzytkownika.dt;

    const int k = 0.01*dzielnik;
    dzielnik = k/dt;

    running = true;

}

float isa_air_density(float H){
    float p;
    p = 1.2255*pow((1-(H/44300)),4.256);
    return p;
}

std::vector<float> PhysicsEngine::derive(){
    TerraCan.pos.x = x_start;
    TerraCan.pos.y = y_start;
    TerraCan.pos.z = z_start;

    TerraCan.vel.x = vx_start;
    TerraCan.vel.y = vy_start;
    TerraCan.vel.z = vz_start;
    std::vector<float> wyniki;
    float V_total, Fd, p;
    const float Fg = -(TerraCan.mass * g);

    for (int i = 0; i < timeout; i++)
    {
        float t = i * dt;   //aktualna sekunda
        V_total = sqrt(pow(TerraCan.vel.x,2)+pow(TerraCan.vel.y,2)+pow(TerraCan.vel.z,2));  // wartosc wektora predkosci - predkosc wypadkowa
        p = isa_air_density(TerraCan.pos.z);

        if(t>chute_delay){                                                                           // warunek otwarcia spadochronu
            Fd = 0.5*TerraCan.Cd*p*TerraCan.A*pow(V_total,2) + 0.5*0.8*p*0.1*pow(V_total,2);//wypadkowy opor powietrza
        }else{
            Fd = 0.5*TerraCan.Cd*p*TerraCan.A*pow(V_total,2);
        }

        if(V_total > 0){                                                                    // w pierwszej sekundzie symulacji predkosc = 0 - zabezpieczenie przed /0
            TerraCan.F.x = 0 - (TerraCan.vel.x/V_total)*Fd;                                 //skalowanie oporu powietrza dla kazdej z osi proporcja do predkosci
            TerraCan.F.y = 0 - (TerraCan.vel.y/V_total)*Fd;
            TerraCan.F.z = Fg - (TerraCan.vel.z/V_total)*Fd;
        } else {
            TerraCan.F.x = 0;
            TerraCan.F.y = 0;
            TerraCan.F.z = Fg;
        }

        //obliczanie kolejno przyspieszenia w danej sekundzie, predkosci calkowitej dla kazdej z osi, oraz przemieszczenia

        TerraCan.acc.x = TerraCan.F.x/TerraCan.mass;
        TerraCan.acc.y = TerraCan.F.y/TerraCan.mass;
        TerraCan.acc.z = TerraCan.F.z/TerraCan.mass;

        TerraCan.vel.x += TerraCan.acc.x * dt;
        TerraCan.vel.y += TerraCan.acc.y * dt;
        TerraCan.vel.z += TerraCan.acc.z * dt;

        //obliczanie pochylenia puszki z tg

        if(TerraCan.vel.x == 0){
            TerraCan.rot.y = 90;
        } else{
            TerraCan.rot.y = abs(atan(TerraCan.vel.z/TerraCan.vel.x)) * (180/pi); //tg kata w osi y
        }

        if(TerraCan.vel.y == 0){
            TerraCan.rot.x = 90;
        } else{
            TerraCan.rot.x = abs(atan(TerraCan.vel.z/TerraCan.vel.y)) * (180/pi); //tg kata w osi x
        }


        TerraCan.pos.x += TerraCan.vel.x * dt;
        TerraCan.pos.y += TerraCan.vel.y * dt;
        TerraCan.pos.z += TerraCan.vel.z * dt;

        wyniki.push_back(TerraCan.pos.x);
        wyniki.push_back(TerraCan.pos.y);
        wyniki.push_back(TerraCan.pos.z);

        wyniki.push_back(TerraCan.vel.x);
        wyniki.push_back(TerraCan.vel.y);
        wyniki.push_back(TerraCan.vel.z);

        wyniki.push_back(TerraCan.rot.x);
        wyniki.push_back(TerraCan.rot.y);

        QString line = "X: " + QString::number(TerraCan.pos.x, 'f', 2) + " Y: " + QString::number(TerraCan.pos.y, 'f', 2) + " Z: " + QString::number(TerraCan.pos.z, 'f', 2)
            + " vX: " + QString::number(TerraCan.vel.x, 'f', 2) + " vY: " + QString::number(TerraCan.vel.y, 'f', 2) + " vZ: " + QString::number(TerraCan.vel.z, 'f', 2)
            + " rX: " + QString::number(TerraCan.rot.x, 'f', 2) + " rY: " + QString::number(TerraCan.rot.y, 'f', 2) + " Czas: " + QString::number(t, 'f', 2);

        if(i%dzielnik == 0){
            emit krokPoliczony(line);
            QThread::msleep(1);
        }
        if(!running) break;
        if(TerraCan.pos.z < 0 ) break;
    }
    emit skonczylem();
    return wyniki;
}

void PhysicsEngine::zatrzymaj(){
    running = false;
}