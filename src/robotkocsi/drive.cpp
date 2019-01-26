#include "drive.h"
#include <math.h>

Drive::Drive() {}
Drive::Drive(PinName pF, PinName pB, PinName pS, Lights* lights) {
    // create Pwm and Servo objects
    po_forward = new PwmOut(pF);
    po_backward = new PwmOut(pB);
    po_steering = new PwmOut(pS);
    
    this->lights = lights;
    autoIndex = (lights)?true:false;
    enabled = true;
    
    po_forward->period(0.0001); // 10kHz
    po_backward->period(0.0001); // 10kHz
    po_steering->period(0.01); // 100Hz

    // initialize variables
    f_forward = false;
    f_backward = false;
    steeringTargetPosition = steerCenter;
    
    controlThread = new Thread(controlThread_main, this);

    printf("Drive created\n");
}

Drive::~Drive() {
    delete po_forward;
    delete po_backward;
    delete po_steering;
}

/**
 * motor control thread
 */
void Drive::controlThread_main(void const *argument) {
    Drive* self = (Drive*)argument;

    float forwardPower = 0.0f;
    float backwardPower = 0.0f;
    bool motorDebug = false;
    
    float steeringServoPosition = steerCenter;
    self->steeringStep = steeringNormStep;
    bool steeringDebug = false;

    // center steering
    self->po_steering->write(steeringServoPosition);

    // 100 Hz control loop
    while (true) {
        // main drive control
        if(self->f_forward) {
            // motorDebug = true;
            if(forwardPower < 0.1f)
                forwardPower = 1.0f;  // initial torque
            else {
                if(forwardPower > 0.2f)
                    forwardPower -= 0.05f; // decreasing torque to a constant 20%
            }
        } else
            forwardPower = 0.0f;
        if(self->f_backward) {
            // motorDebug = true;
            if(backwardPower < 0.1f)
                backwardPower = 1.0f;  // initial torque
            else {
                if(backwardPower > 0.2f)
                    backwardPower -= 0.05f; // decreasing torque to a constant 20%
            }
        } else
            backwardPower = 0.0f;

        self->po_forward->write(forwardPower);
        self->po_backward->write(backwardPower);
        if(motorDebug)
            printf("Motor power: %f, %f\n", forwardPower, backwardPower);
        if(!(self->f_forward) && !(self->f_backward))
            motorDebug = false;

        // steering control
        if(fabs(self->steeringTargetPosition - steeringServoPosition) > 0.001f) {
            steeringDebug = true;
            if(self->steeringTargetPosition > steeringServoPosition)
                steeringServoPosition += self->steeringStep;
            else
                steeringServoPosition -= self->steeringStep;
            if(steeringDebug)
                printf("Steering position (target and actual): %f, %f\n", self->steeringTargetPosition, steeringServoPosition);
            self->po_steering->write(steeringServoPosition);
        }

        Thread::wait(10);   // 100 Hz
    }
}

void Drive::forward() {
    if(!enabled)
        return;
    if(lights)
        lights->reversingLightOff();
    f_backward = false;
    f_forward = true;
}

void Drive::backward() {
    if(!enabled)
        return;
    if(lights)
        lights->reversingLightOn();
    f_forward = false;
    f_backward = true;
}

void Drive::stop() {
    if(lights)
        lights->reversingLightOff();
    f_forward = false;
    f_backward = false;
}

void Drive::steerLeft(float target, float speed) {
    if(!enabled)
        return;
    if(target >= steerCenter)
        return;
    if(lights && autoIndex)
        lights->indexLeft();
    steeringTargetPosition = target;
    steeringStep = speed;
}

void Drive::steerRight(float target, float speed) {
    if(!enabled)
        return;
    if(target <= steerCenter)
        return;
    if(lights && autoIndex)
        lights->indexRight();
    steeringTargetPosition = target;
    steeringStep = speed;
}

void Drive::steerStraight(float speed) {
    if(lights && autoIndex)
        lights->indexOff();
    steeringTargetPosition = steerCenter;
    steeringStep = speed;
}

void Drive::setAutoIndex(bool autoIndex) {
    this->autoIndex = autoIndex;
}

bool Drive::setEnabled(bool enabled) {
    if(this->enabled == enabled)
        return false;
        
    this->enabled = enabled;
    if(!enabled) {
        printf("Disabling drive...\n");
        stop();
        steerStraight();
    } else
        printf("Enabling drive...\n");
    
    return true;    // return true on change
}
