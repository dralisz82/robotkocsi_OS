#ifndef temperaturesensor_H
#define temperaturesensor_H

#include "sensors.h"
#include "DS1820.h"

#define MAX_NUM_DS  5

class TemperatureSensor : public Sensor {

public:
    TemperatureSensor(char *sId, char *name, char *metric, int probeId) : Sensor(sId, name, metric), probeId(probeId) { init(); }
    void init();
    void convertTemperature(bool wait = false);
    DS1820* getTempProbe(int probeId);
    virtual float readValue();

private:
    int probeId;
    static DS1820* ds1820Probe[MAX_NUM_DS];
    static bool ds1820SearchingDone;
};

#endif
