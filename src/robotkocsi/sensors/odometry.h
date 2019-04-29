#ifndef odometry_H
#define odometry_H

#include "sensors.h"

#define WHEELCIRCUMFERENCE 345.0
#define PPR 12

class Odometry : public Sensor {

public:
    Odometry(char *sId, char *name, char *metric) : Sensor(sId, name, metric) { init(); }
    void init();
    virtual float readValue();
    
private:
    InterruptIn *hallA, *hallB;
    long odoCount;
    enum HallID {X, A, B};
    enum Direction {FW, BW};
    HallID lastHall;
    Direction dir;
    
    void hallAISR(void);
    void hallBISR(void);
    void toggleDir();
};

#endif
