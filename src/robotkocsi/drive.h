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
    bool f_forward;
    bool f_backward;
    bool f_steerLeft;
    bool f_steerRight;

    PwmOut* po_forward;
    PwmOut* po_backward;
    PwmOut* po_steerLeft;
    PwmOut* po_steerRight;
    Lights* lights;
    bool autoIndex;

    Thread *controlThread;
    static void controlThread_main(void const *argument);
};
    
#endif
