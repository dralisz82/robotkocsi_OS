#ifndef drive_H
#define drive_H

#include "mbed.h"
#include "lights.h"

class Drive {
public:
    Drive();
    Drive(PinName pF, PinName pB, PinName pS, Lights* lights=NULL);
    ~Drive();
    
    void forward();
    void backward();
    void stop();
    void steerLeft(float target = 0.057f); // 1.14 ms
    void steerRight(float target = 0.073f); // 1.46 ms
    void steerStraight();
    void setAutoIndex(bool autoIndex);
    void setEnabled(bool enabled);

private:
    bool f_forward;
    bool f_backward;
    float steeringTargetPosition;

    PwmOut* po_forward;
    PwmOut* po_backward;
    PwmOut* po_steering;
    Lights* lights;
    bool autoIndex;
    bool enabled;

    Thread *controlThread;
    static void controlThread_main(void const *argument);
};
    
#endif
