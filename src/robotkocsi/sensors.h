#ifndef sensors_H
#define sensors_H

#include "mbed.h"
#include <FunctionPointer.h>
#include "DS1820.h"

#define MAX_NUM_SENS 10
#define MAX_NUM_DS  5

enum SensorID { SENS_VBATT, SENS_VMC, SENS_VLOGIC, SENS_TEMP1, SENS_TEMP2, SENS_ODO };

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
    DS1820* getTempProbe(int pId);
    void convertTemperature(bool wait = false);
    void incOdo();
    int readOdo();

private:
    void createVBatt();
//    void createVServo();
    void createV33();
    void createVLogic();
    void createTempSens();
    void createOdometry();
    
    Sensor sensArr[MAX_NUM_SENS];
    int sensNum;
    
    AnalogIn* analogInputs[6];

    DS1820* ds1820Probe[MAX_NUM_DS];
    bool ds1820SearchingDone;

    InterruptIn* hall;
    int odoCount;
};
    
#endif
