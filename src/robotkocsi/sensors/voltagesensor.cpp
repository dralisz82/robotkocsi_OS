#include "voltagesensor.h"

extern Sensors* sensors;

float VoltageSensor::readValue() {
    return sensors->getAnalogIn(0)->read() * factor;
}
