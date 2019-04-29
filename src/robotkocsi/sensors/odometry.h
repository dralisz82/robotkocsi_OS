#ifndef odometry_H
#define odometry_H

#include "sensors.h"

#define WHEELCIRCUMFERENCE 345.0
#define PPR 12

class Odometry : public Sensor {

public:
    enum readingID { Distance, RelativePosition, CurrentSpeed, AvgSpeed, HighestSpeed };
    Odometry(char *sId, char *name, char *metric) : Sensor(sId, name, metric) { init(); }
    void init();
    virtual unsigned int getNumberOfReadings();
    virtual char* getReadingName(unsigned int readingId = 0);
    virtual char* getMetric(unsigned int readingId = 0);
    virtual float readValue(unsigned int readingId = 0);
    
private:
    enum HallID {X, A, B};
    enum Direction {FW, BW};

    // ISRs and other internal stuff
    InterruptIn *hallA, *hallB;
    Timer timer;
    void hallAISR(void);
    void hallBISR(void);
    void toggleDir();
    void calcSpeed(HallID actHall);
    HallID lastHall;    // ID of sensor, triggered last
    Direction dir;      // Direction of rotation
    
    // Internal variables to measure
    unsigned long previousTime;
    float previousSpeed;    // (cm/sec)
    float speedSum;

    // Published variables
    long odoCount;  // Total pulses since start
    long odoPos;    // Relative pulses since start
    float currentSpeed;
    float avgSpeed;
    float highestSpeed;
};

#endif
