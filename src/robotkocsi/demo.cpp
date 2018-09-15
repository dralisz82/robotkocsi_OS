#include "demo.h"

Demo::Demo() {}
Demo::Demo(Drive* drive, Lights* lights) {
    this->drive = drive;
    this->lights = lights;

    demoThread = 0;
    // initialize flags
    f_run = false;
    printf("Demo created\n");
}

Demo::~Demo() {
    delete demoThread;
}

void Demo::demoThread_main(void const *argument) {
    Demo* self = (Demo*)argument;
    printf("Demo thread started\n");
    
    Thread::wait(10000);
    self->lights->headLightOn();
    Thread::wait(1000);
        self->drive->forward();
        Thread::wait(500);
    while(true) {
    while(self->f_run) {
        self->drive->forward();
        self->drive->steerLeft();
        Thread::wait(1000);
        self->drive->stop();
        Thread::wait(500);
        self->drive->steerRight();
        self->drive->backward();
        Thread::wait(1500);
        self->drive->stop();
        Thread::wait(500);
    }
        printf("Demo thread paused\n");
        Thread::wait(1000);
    }
}

void Demo::playPause() {
    f_run = !f_run;
    if(demoThread == 0) {
        printf("Launch Demo thread\n");
        demoThread = new Thread(demoThread_main, this);    
    }
}

void Demo::reset() {
    delete demoThread;
    f_run = false;
    printf("Demo thread terminated\n");
    lights->headLightOff();
}
