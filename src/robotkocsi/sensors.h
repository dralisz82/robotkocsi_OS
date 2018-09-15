#ifndef sensors_H
#define sensors_H

#include "mbed.h"
#include <FunctionPointer.h>
#define MAX_NUM_SENS 10
#define SENS_VBATTS1 0
#define SENS_VBATTS2 1
#define SENS_VBATTS3 2
#define SENS_VBATT 3
#define SENS_VMOTOR 4
#define SENS_VLOGIC 5


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
    void createS1();
    void createS2();
    void createS3();
    void createVB();
    void createVM();
    void createVL();
    
    Sensor sensArr[MAX_NUM_SENS];
    int sensNum;
    
    AnalogIn* analogInputs[6];
};
    
#endif
