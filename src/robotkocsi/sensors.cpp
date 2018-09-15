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
    createS1();
    createS2();
    createS3();
    createVB();
    createVM();
    createVL();
}

Sensors::~Sensors() {
    for(int i=0;i<=5;i++)
        delete analogInputs[i];
}

Sensor* Sensors::getSensor(char *sId) {
    if(!strcmp(sId, "vBattS1"))
        return &sensArr[SENS_VBATTS1];
    else if(!strcmp(sId, "vBattS2"))
        return &sensArr[SENS_VBATTS2];
    else if(!strcmp(sId, "vBattS3"))
        return &sensArr[SENS_VBATTS3];
    else if(!strcmp(sId, "vBatt"))
        return &sensArr[SENS_VBATT];
    else if(!strcmp(sId, "vMotor"))
        return &sensArr[SENS_VMOTOR];
    else if(!strcmp(sId, "vLogic"))
        return &sensArr[SENS_VLOGIC];
    return NULL;
}

AnalogIn* Sensors::getAnalogIn(int aiId) {
    if(aiId >= 0 && aiId <= 5)
        return analogInputs[aiId];
    return NULL;
}

float readS1() {
    float factor = 3.3;// TODO
    return sensors->getAnalogIn(0)->read()*factor;
}

void Sensors::createS1() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vBattS1");
    s->setName("battery cell #1 voltage");
    s->setMetric("V");
    s->setFunction(readS1);
}

float readS2() {
    float factor = 3.3;// TODO
    return (sensors->getAnalogIn(1)->read() - sensors->getAnalogIn(0)->read())*factor;
}

void Sensors::createS2() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vBattS2");
    s->setName("battery cell #2 voltage");
    s->setMetric("V");
    s->setFunction(readS2);
}

float readS3() {
    float factor = 3.3;// TODO
    return (sensors->getAnalogIn(2)->read() - sensors->getAnalogIn(1)->read())*factor;
}

void Sensors::createS3() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vBattS3");
    s->setName("battery cell #3 voltage");
    s->setMetric("V");
    s->setFunction(readS3);
}

float readVB() {
    float factor = 3.3;// TODO
    return sensors->getAnalogIn(2)->read()*factor;
}

void Sensors::createVB() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vBatt");
    s->setName("battery voltage");
    s->setMetric("V");
    s->setFunction(readVB);
}

float readVM() {
    float factor = 3.3;// TODO
    return sensors->getAnalogIn(3)->read()*factor;
}

void Sensors::createVM() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vMotor");
    s->setName("motor voltage");
    s->setMetric("V");
    s->setFunction(readVM);
}

float readVL() {
    float factor = 3.3;// TODO
    return sensors->getAnalogIn(4)->read()*factor;
}

void Sensors::createVL() {
    Sensor *s = &sensArr[sensNum++];
    s->setId("vLogic");
    s->setName("logic voltage");
    s->setMetric("V");
    s->setFunction(readVL);
}
