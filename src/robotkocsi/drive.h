#ifndef drive_H
#define drive_H

#include "mbed.h"
#include "lights.h"

// 1.12 1.3 1.48 ms
#define steerMaxLeft    0.056f
#define steerCenter     0.065f
#define steerMaxRight   0.074f

class Drive {
public:
    Drive();
    Drive(PinName pF, PinName pB, PinName pS, Lights* lights=NULL);
    ~Drive();
    
    void forward();
    void backward();
    void stop();
    void steerLeft(float target = steerMaxLeft);
    void steerRight(float target = steerMaxRight);
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
