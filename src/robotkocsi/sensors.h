#ifndef sensors_H
#define sensors_H

#include "mbed.h"
#include <FunctionPointer.h>
#define MAX_NUM_SENS 10

enum SensorID { SENS_VBATT, SENS_VMC, SENS_VLOGIC };

class Sensors;

class Sensor {
public:
    Sensor();
    Sensor(char *sId, char *name, char *metric);
    ~Sensor();
    void setId(char *sId);
    void setName(char *name);
    void setMetric(char *metric);
    void setFunction(float (*function)(void) = 0);
    char *getId();
    char *getName();
    char * getMetric();
    float readValue();
    
private:
    char sId[10];
    char name[30];
    char metric[4]; // max 3 characters + terminating 0
    float (*function)(void);
};

class Sensors {
public:
    Sensors();
    ~Sensors();
    Sensor* getSensor(char *sId);
    AnalogIn* getAnalogIn(int aiId);

private:
    void createVBatt();
//    void createVServo();
    void createV33();
    void createVLogic();
    
    Sensor sensArr[MAX_NUM_SENS];
    int sensNum;
    
    AnalogIn* analogInputs[6];
};
    
#endif
