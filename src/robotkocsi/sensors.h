#ifndef sensors_H
#define sensors_H

#include "mbed.h"

#define MAX_NUM_SENS 10

#define SENSORIDLEN 10
#define SENSORNAMELEN 30
#define SENSORMETRICLEN 4

enum SensorID { SENS_VBATT, SENS_VMC, SENS_VLOGIC, SENS_TEMP1, SENS_TEMP2, SENS_ODO };

class Sensor {
    
public:
    Sensor();
    Sensor(char *sId, char *name, char *metric);
    ~Sensor();
    char* getId();
    virtual void setReadingName(char *name, unsigned int readingId = 0);
    virtual char* getReadingName(unsigned int readingId = 0);
    virtual char* getMetric(unsigned int readingId = 0);
    virtual float readValue(unsigned int readingId = 0);
    virtual unsigned int getNumberOfReadings();
//    char* stringData();
    
protected:
    char sId[SENSORIDLEN];
    char name[SENSORNAMELEN];
    char metric[SENSORMETRICLEN]; // max 3 characters + terminating 0
};

class Sensors {
    
public:
    Sensors();
    ~Sensors();
    void addSensor(Sensor *sensor);
    Sensor* getSensor(char *sId);
    AnalogIn* getAnalogIn(int aiId);
    
private:    
    Sensor* sensArr[MAX_NUM_SENS];
    int sensNum;
    
    AnalogIn* analogInputs[6];
};
    
#endif
