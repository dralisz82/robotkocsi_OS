#ifndef sonar_H
#define sonar_H

#include "sensors.h"

/**
 *  Sonar class for the HC-SR04
 */
class Sonar : public Sensor {

public:
    /**
     *  Sonar constructor
     *  Creates a sonar object on a set of provided pins
     *  @param trigger_pin  Pin used to trigger reads from the sonar device
     *  @param echo_pin     Pin used to receive the sonar's distance measurement
     */
    Sonar(char *sId, char *name, char *metric, PinName trigger_pin, PinName echo_pin)  : Sensor(sId, name, metric), trigger(trigger_pin), echo(echo_pin) { init(); }
    virtual float readValue(unsigned int readingId = 0);
    void start(void);
    void stop(void);
    
private:
    void init();
    void echo_in(void);
    void echo_fall(void);
    void trigger_toggle(void);
    void background_read(void);

    DigitalOut   trigger;
    InterruptIn  echo;     // calls a callback when a pin changes
    Timer        timer;
    Timeout      timeout;  // calls a callback once when a timeout expires
    Ticker       ticker;   // calls a callback repeatedly with a timeout
    int32_t      begin;
    int32_t      end;
    float        distance;
};

#endif
