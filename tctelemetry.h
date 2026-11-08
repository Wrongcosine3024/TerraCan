#include <cstdint>
#ifndef TCTELEMETRY_H
#define TCTELEMETRY_H

static uint8_t checksum(const uint8_t* data, size_t size){
    uint8_t sum = 0;
    for (size_t i = 0; i < size; i++)
    {
        sum ^= data[i];
    }
    return sum;
}

#endif // TCTELEMETRY_H

#pragma pack(push, 1)

struct TELEMETRY{
    uint32_t SEQ;
    uint32_t timestamp;
    uint8_t state;
    float GPS_lat;
    float GPS_long;
    float GPS_alt;
    float GPS_speed_N;
    float GPS_speed_E;
    float GPS_speed_V;
    float temperature;
    float pressure;
    float acc_x, acc_y, acc_z;     // akcjeleracja w 3 osiach
    float rot_x, rot_y, rot_z;     // predkosc katowa w 3 osiach
};

struct packet{
    uint8_t magic1;     //0xAA
    uint8_t magic2;     //0xBB
    TELEMETRY payload;
    uint8_t crc;        //suma kontrolna
};
// struct TRACKING{
//     unsigned int SEQ;
//     unsigned int timestamp;
//     double GPS_lat;
//     double GPS_long;
//     double GPS_alt;
// };

// struct PHOTO{
//     unsigned int SEQ;
//     unsigned int timestamp;
//     bool if_success;
// };

#pragma pack(pop)