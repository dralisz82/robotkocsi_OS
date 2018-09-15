#include "drive.h"

Drive::Drive() {}
Drive::Drive(PinName pF, PinName pB, PinName pL, PinName pR, Lights* lights) {
    // create Pwm and DigitalOut objects
    po_forward = new PwmOut(pF);
    po_backward = new PwmOut(pB);
    po_steerLeft = new PwmOut(pL);
    po_steerRight = new PwmOut(pR);
    
    this->lights = lights;
    autoIndex = (lights)?true:false;
    
    po_forward->period(0.0001); // 10kHz
    po_backward->period(0.0001); // 10kHz
    po_steerLeft->period(0.0001); // 10kHz
    po_steerRight->period(0.0001); // 10kHz

    // initialize flags
    f_forward = false;
    f_backward = false;
    f_steerLeft = false;
    f_steerRight = false;
    
    controlThread = new Thread(controlThread_main, this);

    printf("Drive created\n");
}

Drive::~Drive() {
    delete po_forward;
    delete po_backward;
    delete po_steerLeft;
    delete po_steerRight;
}

/**
 * motor control thread
 */
void Drive::controlThread_main(void const *argument) {
    Drive* self = (Drive*)argument;

    float forwardPower = 0.0f;
    float backwardPower = 0.0f;
    bool motorDebug = false;
    
    float steerLeftPower = 0.0f;
    float steerRightPower = 0.0f;
    bool steeringDebug = false;
    
    // 20 Hz control loop
    while (true) {
        if(self->f_forward) {
            motorDebug = true;
            if(forwardPower < 0.1f)
                forwardPower = 1.0f;  // initial torque
            else {
                if(forwardPower > 0.5f)
                    forwardPower -= 0.1f; // decreasing torque to a constant 50%
            }
        } else
            forwardPower = 0.0f;
        if(self->f_backward) {
            motorDebug = true;
            if(backwardPower < 0.1f)
                backwardPower = 1.0f;  // initial torque
            else {
                if(backwardPower > 0.7f)
                    backwardPower -= 0.1f; // decreasing torque to a constant 70%
            }
        } else
            backwardPower = 0.0f;

        self->po_forward->write(forwardPower);
        self->po_backward->write(backwardPower);
        if(motorDebug)
            printf("Motor power: %f, %f\n", forwardPower, backwardPower);
        if(!(self->f_forward) && !(self->f_backward))
            motorDebug = false;

        if(self->f_steerLeft) {
            steeringDebug = true;
            if(steerLeftPower < 0.1f)
                steerLeftPower = 1.0f;  // initial torque
            else {
                if(steerLeftPower > 0.9f)
                    steerLeftPower -= 0.02f; // decreasing torque to a constant 90%
            }
        } else
            steerLeftPower = 0.0f;
        if(self->f_steerRight) {
            steeringDebug = true;
            if(steerRightPower < 0.1f)
                steerRightPower = 1.0f;  // initial torque
            else {
                if(steerRightPower > 0.9f)
                    steerRightPower -= 0.02f; // decreasing torque to a constant 90%
            }
        } else
            steerRightPower = 0.0f;

        self->po_steerLeft->write(steerLeftPower);
        self->po_steerRight->write(steerRightPower);
        if(steeringDebug)
            printf("Steering power: %f, %f\n", steerLeftPower, steerRightPower);
        if(!(self->f_steerLeft) && !(self->f_steerRight))
            steeringDebug = false;

        Thread::wait(50);
    }
}

void Drive::forward() {
    if(lights)
        lights->reversingLightOff();
    f_backward = false;
    f_forward = true;
}

void Drive::backward() {
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

void Drive::steerLeft() {
    if(lights && autoIndex)
        lights->indexLeft();
    f_steerRight = false;
    f_steerLeft = true;
}

void Drive::steerRight() {
    if(lights && autoIndex)
        lights->indexRight();
    f_steerLeft = false;
    f_steerRight = true;
}

void Drive::steerStraight() {
    if(lights && autoIndex)
        lights->indexOff();
    f_steerLeft = false;
    f_steerRight = false;
}

void Drive::setAutoIndex(bool autoIndex) {
    this->autoIndex = autoIndex;
}
