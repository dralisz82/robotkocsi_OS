#include "sensors.h"
#include "voltagesensor.h"
#include "temperaturesensor.h"
#include "odometry.h"
#include "sonar.h"

extern Sensors* sensors;

Sensor::Sensor() {
}

Sensor::Sensor(char *sId, char *name, char *metric) {
    strncpy(this->sId, sId, SENSORIDLEN);
    this->name[SENSORIDLEN-1] = '\0';
    strncpy(this->name, name, SENSORNAMELEN);
    this->name[SENSORNAMELEN-1] = '\0';
    strncpy(this->metric, metric, SENSORMETRICLEN);
    this->name[SENSORMETRICLEN-1] = '\0';
}

Sensor::~Sensor() {
}

void Sensor::setReadingName(char *name, unsigned int readingId) {
    strncpy(this->name, name, SENSORNAMELEN);
    this->name[SENSORNAMELEN-1] = '\0';
}

char* Sensor::getId() {
    return sId;
}

char* Sensor::getReadingName(unsigned int readingId) {
    return name;
}

char* Sensor::getMetric(unsigned int readingId) {
    return metric;
}

float Sensor::readValue(unsigned int readingId) {
    return 0.0f;
}

unsigned int Sensor::getNumberOfReadings() {
    return 1;
}

Sensors::Sensors() {
    // create peripheral objects
    analogInputs[0] = new AnalogIn(PTB2); // A0
    analogInputs[1] = new AnalogIn(PTB3); // A1
    analogInputs[2] = new AnalogIn(PTB10); // A2
    analogInputs[3] = new AnalogIn(PTB11); // A3
    analogInputs[4] = new AnalogIn(PTC11); // A4
    analogInputs[5] = new AnalogIn(PTC10); // A5
    
    sensNum = 0;

    // create sensor objects
    addSensor(new VoltageSensor("vBatt", "battery voltage", "V", 0, 3.3 * 8.25));   // Adjusted value of theoretical: 3.3V * 115k / 15k
    addSensor(new VoltageSensor("vMC", "uC voltage", "V", 1, 3.3));   // Using it temporarily to measure 3.3 V supply of controller
    addSensor(new VoltageSensor("vLogic", "logic voltage", "V", 2, 3.3 * 2.08));   // Adjusted value of theoretical: 3.3V * 94k / 47k
    addSensor(new TemperatureSensor("Temp1", "ambient temperature", "°C", 0));
    addSensor(new TemperatureSensor("Temp2", "power supply temperature", "°C", 1));
    addSensor(new Odometry("Odo", "odometry", "cm"));
    addSensor(new Sonar("sonFront", "front sonar", "cm", PTD3, PTD1));  // trigger: D12, echo: D13
    ((Sonar*)sensArr[SENS_SONAR])->start();
}

Sensors::~Sensors() {
    for(int i=0;i<sensNum;i++)
        delete sensArr[i];

    for(int i=0;i<=5;i++)
        delete analogInputs[i];
}

void Sensors::addSensor(Sensor *sensor) {
    if(sensNum < MAX_NUM_SENS)
        sensArr[sensNum++] = sensor;
}

Sensor* Sensors::getSensor(char *sId) {
    printf("getSensor()\n");
    if(!strcmp(sId, "vBatt"))
        return sensArr[SENS_VBATT];
//    else if(!strcmp(sId, "vServo"))
//        return &sensArr[SENS_VSERVO];
    else if(!strcmp(sId, "vMC"))
        return sensArr[SENS_VMC];
    else if(!strcmp(sId, "vMotor"))
        return sensArr[SENS_VBATT];
    else if(!strcmp(sId, "vLogic"))
        return sensArr[SENS_VLOGIC];
    else if(!strcmp(sId, "tAmbient"))
        return sensArr[SENS_TEMP1];
    else if(!strcmp(sId, "tPS"))
        return sensArr[SENS_TEMP2];
    else if(!strcmp(sId, "odo"))
        return sensArr[SENS_ODO];
    else if(!strcmp(sId, "sonFront"))
        return sensArr[SENS_SONAR];
    return NULL;
}

AnalogIn* Sensors::getAnalogIn(int aiId) {
    if(aiId >= 0 && aiId <= 5)
        return analogInputs[aiId];
    return NULL;
}
