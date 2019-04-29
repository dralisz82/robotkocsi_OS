#include "temperaturesensor.h"

void TemperatureSensor::init() {
/*    int i, devices_found = 0;
    
    ds1820SearchingDone = false;
    
    // Initialize the probe array to DS1820 objects
    while(DS1820::unassignedProbe(PTC12)) { // D8
        ds1820Probe[devices_found] = new DS1820(PTC12); // D8
        devices_found++;
        if (devices_found == MAX_NUM_DS)
            break;
    }
    
    printf("%d DS1820  probes found.\n", devices_found);
    ds1820SearchingDone = true;
    
    ds1820Probe[0]->convertTemperature(true, DS1820::all_devices);
    printf("Temperature converted.\n");
    for(i=0; i<devices_found; i++)
        printf("Temperature %d: %f°C\n", i, ds1820Probe[i]->temperature('c'));
*/}

void TemperatureSensor::convertTemperature(bool wait) {
//    if(ds1820SearchingDone)
//        ds1820Probe[0]->convertTemperature(wait, DS1820::all_devices);
}

DS1820* TemperatureSensor::getTempProbe(int probeId) {
//    return ds1820Probe[probeId];
}

float TemperatureSensor::readValue(unsigned int readingId) {
    return getTempProbe(probeId)->temperature('c');
}
