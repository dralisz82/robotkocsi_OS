#ifndef drive_H
#define drive_H

#include "mbed.h"
#include "lights.h"
#include "sensors.h"
#include "sensors/odometry.h"

// 1.12 1.3 1.48 ms - old servo
// 1.32 1.5 1.68 ms - new servo
#define steerMaxLeft        0.131f
#define steerCenter         0.15f
#define steerMaxRight       0.169f
#define steeringNormStep    0.002f
#define driveDefaultSpeed      0.3f

class Drive {
public:
    Drive();
    Drive(PinName pF, PinName pB, PinName pS, Lights* lights=NULL, Sensors* sensors=NULL);
    ~Drive();
    
    void forward();
    void backward();
    void stop();
    void brake();
    void releaseBrake();
    void steerLeft(float target = steerMaxLeft, float speed = steeringNormStep);
    void steerRight(float target = steerMaxRight, float speed = steeringNormStep);
    void steerStraight(float speed = steeringNormStep);
    void setAutoIndex(bool autoIndex);
    bool setEnabled(bool enabled);
    float setSpeed(float speed);

private:
    bool f_forward;
    bool f_backward;
    bool f_brake;
    float steeringTargetPosition;
    float steeringStep;
    float driveSpeed;

    PwmOut* po_forward;
    PwmOut* po_backward;
    PwmOut* po_steering;
    Lights* lights;
    Sensor* odometry;
    Sensor* frontSonar;
    bool autoIndex;
    bool enabled;

    Thread *controlThread;
    static void controlThread_main(void const *argument);
};
    
#endif
