#ifndef TCTELEMETRY_H
#define TCTELEMETRY_H

#endif // TCTELEMETRY_H

struct IMU{
    double acc_x, acc_y, acc_z;     // akcjeleracja w 3 osiach
    double rot_x, rot_y, rot_z;     // PRĘDKOŚĆ kątowa w 3 osiach
};

struct TELEMETRY{
    unsigned int SEQ;
    unsigned int timestamp;
    double GPS_lat;
    double GPS_long;
    double GPS_alt;
    float temperature;
    float pressure;
    IMU imu_data;
    unsigned short CRC;
};

struct TRACKING{
    unsigned int SEQ;
    unsigned int timestamp;
    double GPS_lat;
    double GPS_long;
    double GPS_alt;
};

struct PHOTO{
    unsigned int SEQ;
    unsigned int timestamp;
    bool if_success;
};
