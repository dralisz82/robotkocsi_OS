#include "drive.h"

Drive::Drive() {}
Drive::Drive(PinName pF, PinName pB, PinName pL, PinName pR, Lights* lights) {
    // create Pwm and DigitalOut objects
    po_forward = new PwmOut(pF);
    po_backward = new PwmOut(pB);
    do_steerLeft = new DigitalOut(pL);
    do_steerRight = new DigitalOut(pR);
    
    this->lights = lights;
    autoIndex = (lights)?true:false;
    
    po_forward->period(0.0001); // 10kHz
    po_backward->period(0.0001); // 10kHz

    printf("Drive created\n");
}

Drive::~Drive() {
    delete po_forward;
    delete po_backward;
    delete do_steerLeft;
    delete do_steerRight;
}

void Drive::forward() {
    if(lights)
        lights->reversingLightOff();
    po_backward->write(0.0f);
    po_forward->write(0.85f);
}

void Drive::backward() {
    if(lights)
        lights->reversingLightOn();
    po_forward->write(0.0f);
    po_backward->write(0.99f);
}

void Drive::stop() {
    if(lights)
        lights->reversingLightOff();
    po_forward->write(0.0f);
    po_backward->write(0.0f);
}

void Drive::steerLeft() {
    if(lights && autoIndex)
        lights->indexLeft();
    do_steerRight->write(0);
    do_steerLeft->write(1);
}

void Drive::steerRight() {
    if(lights && autoIndex)
        lights->indexRight();
    do_steerLeft->write(0);
    do_steerRight->write(1);
}

void Drive::steerStraight() {
    if(lights && autoIndex)
        lights->indexOff();
    do_steerLeft->write(0);
    do_steerRight->write(0);
}

void Drive::setAutoIndex(bool autoIndex) {
    this->autoIndex = autoIndex;
}
