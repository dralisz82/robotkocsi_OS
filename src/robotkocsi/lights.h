#ifndef lights_H
#define lights_H

#include "mbed.h"
#include "rtos.h"

class Lights {
public:
    Lights();
    Lights(PinName pIL, PinName pIR, PinName pHL, PinName pHB, PinName pRL, PinName pBL);
    ~Lights();

    void indexLeft(int blinks = 0);
    void indexRight(int blinks = 0);
    void indexOff();

    void hazardLightsOn(int blinks = 0);
    void hazardLightsOff();

    void headLightOn();
    void headLightOff();
    void headLightToggle();

    void highBeamOn();
    void highBeamOff();
    void highBeamToggle();

    void reversingLightOn();
    void reversingLightOff();

    void brakeLightOn();
    void brakeLightOff();

private:
    bool f_indexLeft;
    bool f_indexRight;
    int numBlinks;
    
    DigitalOut* do_indexLeft;
    DigitalOut* do_indexRight;
    DigitalOut* do_headLight;
    DigitalOut* do_highBeam;
    DigitalOut* do_reversingLight;
    DigitalOut* do_brakeLight;
    
    Thread *indexThread;
    static void indexThread_main(void const *argument);
};
    
#endif
