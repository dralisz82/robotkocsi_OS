#ifndef demo_H
#define demo_H

#include "mbed.h"
#include "rtos.h"
#include "drive.h"
#include "lights.h"

class Demo {
public:
    Demo();
    Demo(Drive* _drive, Lights* _lights);
    ~Demo();
    
    void playPause();
    void reset();

private:
    bool f_run;
    
    Drive* drive;
    Lights* lights;
    Thread *demoThread;
    static void demoThread_main(void const *argument);
};
    
#endif
