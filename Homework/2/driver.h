#ifndef __DRIVER_H__
#define __DRIVER_H__

#define MAX_DRIVERS 1000

struct Driver
{
    std::string name;
    bool online = true;
    int station = 0;
    int cnt_rides = 0;
    int total_rating = 0;
    double medium_rating = 0.0;
    int total_distance = 0;
};

#endif // __DRIVER_H__