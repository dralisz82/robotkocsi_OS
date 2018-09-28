#ifndef drive_H
#define drive_H

#include "mbed.h"
#include "lights.h"

#define steerMaxLeft    0.058f
#define steerMaxRight   0.075f
#define steerCenter     0.0665f

class Drive {
public:
    Drive();
    Drive(PinName pF, PinName pB, PinName pS, Lights* lights=NULL);
    ~Drive();
    
    void forward();
    void backward();
    void stop();
    void steerLeft(float target = steerMaxLeft); // 1.16 ms
    void steerRight(float target = steerMaxRight); // 1.50 ms
    void steerStraight();
    void setAutoIndex(bool autoIndex);
    bool setEnabled(bool enabled);

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
