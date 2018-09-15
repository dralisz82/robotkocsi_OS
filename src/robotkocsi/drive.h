#ifndef drive_H
#define drive_H

#include "mbed.h"
#include "lights.h"

class Drive {
public:
    Drive();
    Drive(PinName pF, PinName pB, PinName pL, PinName pR, Lights* lights=NULL);
    ~Drive();
    
    void forward();
    void backward();
    void stop();
    void steerLeft();
    void steerRight();
    void steerStraight();
    
    void setAutoIndex(bool autoIndex);

private:
    PwmOut* po_forward;
    PwmOut* po_backward;
    DigitalOut* do_steerLeft;
    DigitalOut* do_steerRight;
    Lights* lights;
    bool autoIndex;
};
    
#endif
