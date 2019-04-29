#include "voltagesensor.h"

extern Sensors* sensors;

float VoltageSensor::readValue(unsigned int readingId) {
    return sensors->getAnalogIn(0)->read() * factor;
}
