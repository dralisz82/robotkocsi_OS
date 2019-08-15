#ifndef odometry_H
#define odometry_H

#include "sensors.h"

#define WHEELCIRCUMFERENCE 345.0
#define PPR 72

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
    enum Direction {X, FW, BW};

    // ISRs and other internal stuff
    InterruptIn *hallA, *hallB;
    Timer timer;
    Timeout hallTimeout;
    void hallAriseISR(void);
    void hallAfallISR(void);
    void hallBriseISR(void);
    void hallBfallISR(void);
    void calcSpeed();
    void clearSpeed();

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
