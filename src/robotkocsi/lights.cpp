#include "lights.h"

Lights::Lights() {}

Lights::Lights(PinName pIL, PinName pIR, PinName pHL, PinName pHB, PinName pRL, PinName pBL) {
    // create DigitalOut objects
    do_indexLeft = new DigitalOut(pIL);
    do_indexRight = new DigitalOut(pIR);
    do_headLight = new DigitalOut(pHL);
    do_highBeam = new DigitalOut(pHB);
    do_reversingLight = new DigitalOut(pRL);
    do_brakeLight = new DigitalOut(pBL);
    
    // initialize flags
    f_indexLeft = false;
    f_indexRight = false;
    
    indexThread = new Thread(indexThread_main, this);
    printf("Lights created\n");
}

Lights::~Lights() {
    delete indexThread;
    delete do_indexLeft;
    delete do_indexRight;
    delete do_headLight;
    delete do_highBeam;
    delete do_reversingLight;
    delete do_brakeLight;
}

/**
 * blinking indexes
 */
void Lights::indexThread_main(void const *argument) {
    Lights* self = (Lights*)argument;
    
    while (true) {
        if(self->f_indexLeft)
            self->do_indexLeft->write(!self->do_indexLeft->read());
        else
            self->do_indexLeft->write(0);
        if(self->f_indexRight)
            self->do_indexRight->write(!self->do_indexRight->read());
        else
            self->do_indexRight->write(0);
        Thread::wait(500);
    }
}

void Lights::indexLeft() {
    f_indexLeft = true;
    f_indexRight = false;
}

void Lights::indexRight() {
    f_indexLeft = false;
    f_indexRight = true;
}

void Lights::indexOff() {
    f_indexLeft = false;
    f_indexRight = false;
}

void Lights::hazardLightsOn() {
    f_indexLeft = true;
    f_indexRight = true;
}

void Lights::hazardLightsOff() {
    indexOff();
}

void Lights::headLightOn() {
    do_headLight->write(1);
}

void Lights::headLightOff() {
    do_headLight->write(0);
}

void Lights::headLightToggle() {
    do_headLight->write(!do_headLight->read());
}

void Lights::highBeamOn() {
    do_highBeam->write(1);
}

void Lights::highBeamOff() {
    do_highBeam->write(0);
}

void Lights::highBeamToggle() {
    do_highBeam->write(!do_highBeam->read());
}

void Lights::reversingLightOn() {
    do_reversingLight->write(1);
}

void Lights::reversingLightOff() {
    do_reversingLight->write(0);
}

void Lights::brakeLightOn() {
    do_brakeLight->write(1);
}

void Lights::brakeLightOff() {
    do_brakeLight->write(0);
}
