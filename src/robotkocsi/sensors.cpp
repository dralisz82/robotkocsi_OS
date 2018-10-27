#include "sensors.h"

extern Sensors* sensors;

Sensor::Sensor() {
}

Sensor::~Sensor() {
}

void Sensor::setId(char *sId) {
    strcpy(this->sId, sId);
}

void Sensor::setName(char *name) {
    strcpy(this->name, name);
}

void Sensor::setMetric(char *metric) {
    strcpy(this->metric, metric);
}

void Sensor::setFunction(float (*function)(void)) {
    this->function = function;
}

char* Sensor::getId() {
    return sId;
}

char* Sensor::getName() {
    return name;
}

char* Sensor::getMetric() {
    return metric;
}

float Sensor::readValue() {
    return (*function)();
}

Sensors::Sensors() {
    // create peripheral objects
    analogInputs[0] = new AnalogIn(PTB2);
    analogInputs[1] = new AnalogIn(PTB3);
    analogInputs[2] = new AnalogIn(PTB10);
    analogInputs[3] = new AnalogIn(PTB11);
    analogInputs[4] = new AnalogIn(PTC11);
    analogInputs[5] = new AnalogIn(PTC10);
    
    sensNum = 0;

    // create sensor objects
    createVBatt();
//    createVServo();
    createV33();
    createVLogic();
    createTempSens();
}

Sensors::~Sensors() {
    for(int i=0;i<=5;i++)
        delete analogInputs[i];
}

Sensor* Sensors::getSensor(char *sId) {
    if(!strcmp(sId, "vBatt"))
        return &sensArr[SENS_VBATT];
//    else if(!strcmp(sId, "vServo"))
//        return &sensArr[SENS_VSERVO];
    else if(!strcmp(sId, "vMC"))
        return &sensArr[SENS_VMC];
    else if(!strcmp(sId, "vMotor"))
        return &sensArr[SENS_VBATT];
    else if(!strcmp(sId, "vLogic"))
        return &sensArr[SENS_VLOGIC];
    else if(!strcmp(sId, "tAmbient"))
        return &sensArr[SENS_TEMP1];
    else if(!strcmp(sId, "tPS"))
        return &sensArr[SENS_TEMP2];
    return NULL;
}

AnalogIn* Sensors::getAnalogIn(int aiId) {
    if(aiId >= 0 && aiId <= 5)
        return analogInputs[aiId];
    return NULL;
}

float readVBatt() {
    float factor = 3.3 * 8.25;    // Adjusted value of theoretical: 3.3V * 115k / 15k
    return sensors->getAnalogIn(0)->read()*factor;
}

void Sensors::createVBatt() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vBatt");
    s->setName("battery voltage");
    s->setMetric("V");
    s->setFunction(readVBatt);
}

//float readVServo() {
float readV33() {
//    float factor = 3.3 * 2; // Adjusted value of theoretical: 3.3V * 94k / 47k
    float factor = 3.3; // Using it temporarily to measure 3.3 V supply of controller
    return sensors->getAnalogIn(1)->read()*factor;
}

//void Sensors::createVServo() {
void Sensors::createV33() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vMC");
    s->setName("uC voltage");
    s->setMetric("V");
    s->setFunction(readV33);
}

float readVLogic() {
    float factor = 3.3 * 2.08; // Adjusted value of theoretical: 3.3V * 94k / 47k
    return sensors->getAnalogIn(2)->read()*factor;
}

void Sensors::createVLogic() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vLogic");
    s->setName("logic voltage");
    s->setMetric("V");
    s->setFunction(readVLogic);
}

DS1820* Sensors::getTempProbe(int pId) {
    return ds1820Probe[pId];
}

float readTemp1() {
    return sensors->getTempProbe(0)->temperature('c');
}

float readTemp2() {
    return sensors->getTempProbe(1)->temperature('c');
}

void Sensors::convertTemperature(bool wait) {
    if(ds1820SearchingDone)
        ds1820Probe[0]->convertTemperature(wait, DS1820::all_devices);
}

void Sensors::createTempSens() {
    int i, devices_found = 0;
    ds1820SearchingDone = false;
    
    // Initialize the probe array to DS1820 objects
    while(DS1820::unassignedProbe(PTC12)) {
        ds1820Probe[devices_found] = new DS1820(PTC12);
        devices_found++;
        if (devices_found == MAX_NUM_DS)
            break;
    }
    
    printf("%d DS1820  probes found.\n", devices_found);
    ds1820SearchingDone = true;
    
    ds1820Probe[0]->convertTemperature(true, DS1820::all_devices);
    printf("Temperature converted.\n");
    for(i=0;i<devices_found;i++)
        printf("Temperature %d: %f°C\n", i, ds1820Probe[i]->temperature('c'));
  
    Sensor *s = &sensArr[sensNum++];
    s->setId("Temp1");
    s->setName("ambient temperature");
    s->setMetric("°C");
    s->setFunction(readTemp1);
    
    s = &sensArr[sensNum++];
    s->setId("Temp2");
    s->setName("power supply temperature");
    s->setMetric("°C");
    s->setFunction(readTemp2);
}

