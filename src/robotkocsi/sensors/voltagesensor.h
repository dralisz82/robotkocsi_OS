#ifndef voltagesensor_H
#define voltagesensor_H

#include "sensors.h"

class VoltageSensor : public Sensor {

public:
    VoltageSensor(char *sId, char *name, char *metric, int aiId, float factor) : Sensor(sId, name, metric), aiId(aiId), factor(factor) {}
    virtual float readValue();
    
private:
    int aiId;
    float factor;
};

#endif
