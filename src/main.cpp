#include "mbed.h"
#include "rtos.h"
#include "commandhandler.h"
#include "robotkocsi/drive.h"
#include "robotkocsi/lights.h"
#include "robotkocsi/sensors.h"
#include "robotkocsi/sensors/odometry.h"
#include "robotkocsi/demo.h"


DigitalOut *heartbeatLED;
DigitalOut normalLED(LED_GREEN);
DigitalOut errorLED(LED_RED);
DigitalOut cmdExecLED(LED_BLUE);

Lights* lights;
Drive* drive;
Sensors* sensors;
Demo* demo;

Serial pc(USBTX, USBRX);
RawSerial BT(PTC15, PTC14);  // Bluetooth module header
Timeout btConnTimeout;

Thread hbThread;  // heartbeat thread
extern Thread cmdHandlerThread;
Thread tmThread;  // telemetry thread

void onBTtimeout() {
    drive->stop();
}

void execCommand(char *cmd, int argc, simplestr *args) {
    if(!strcmp(cmd, "forward")) {
        if(drive->emergencyBrakingOccured())
            BT.printf("Emergency braking at: %.1f cm with speed: %.1f cm/s\n", drive->getEmergencyBrakingDistance(), drive->getEmergencyBrakingSpeed());
        drive->forward();
    }
    else if(!strcmp(cmd, "backward")) {
        drive->backward();
    }
    else if(!strcmp(cmd, "halt")) {
        drive->stop();
    }
    else if(!strcmp(cmd, "brake")) {
        drive->brake();
    }
    else if(!strcmp(cmd, "nobrake")) {
        if(drive->emergencyBrakingOccured())
            BT.printf("Emergency braking at: %.1f cm with speed: %.1f cm/s\n", drive->getEmergencyBrakingDistance(), drive->getEmergencyBrakingSpeed());
        drive->releaseBrake();
    }
    else if(!strcmp(cmd, "left")) {
        drive->steerLeft();
    }
    else if(!strcmp(cmd, "right")) {
        drive->steerRight();
    }
    else if(!strcmp(cmd, "straight")) {
        drive->steerStraight();
    }
    else if(!strcmp(cmd, "index_left")) {
        lights->indexLeft();
    }
    else if(!strcmp(cmd, "index_right")) {
        lights->indexRight();
    }
    else if(!strcmp(cmd, "index_off")) {
        lights->indexOff();
    }
    else if(!strcmp(cmd, "lights")) {
        lights->headLightToggle();
    }
    else if(!strcmp(cmd, "high_beam")) {
        lights->highBeamToggle();
    }
    else if(!strcmp(cmd, "demo_play")) {
        demo->playPause();
    }
    else if(!strcmp(cmd, "demo_reset")) {
        demo->reset();
    }
    else if(!strcmp(cmd, "readSensor")) {
        Sensor *s = sensors->getSensor(args[0]);
        int rID = 0;
        if(s != NULL) {
            if(argc > 1)
                rID = atoi(args[1]);
            BT.printf("%s: %f %s\n", s->getReadingName(rID), s->readValue(rID), s->getMetric(rID));
        }
    }
    else {
        lights->hazardLightsOn();  // ignore invalid command, put emergency indicator on
    }
    printf("%s %d; %s, %s, %s, %s\n", cmd, argc, args[0], args[1], args[2], args[3]);
    btConnTimeout.detach();
    btConnTimeout.attach(&onBTtimeout, 2);
}

void checkBattery() {
    Sensor *s = sensors->getSensor("vBatt");
    printf("Checking battery...");
    if(s == NULL) {
        printf("[error]\n");
        return;
    }
    if(s->readValue() < 10.2f) {
        printf("[low]\n");
        drive->setEnabled(false);
        lights->hazardLightsOn();
        normalLED = 1;  // off
        heartbeatLED = &errorLED;
        BT.printf("Battery low, drive disabled.\n");
    } else {
        printf("[ok]\n");
    }
    if(s->readValue() > 11.2f) {
        if(drive->setEnabled(true)) {
            lights->hazardLightsOff();
            errorLED = 1;   // off
            heartbeatLED = &normalLED;
            BT.printf("Battery charged, drive enabled.\n");
        }
    }
}

/**
 * Heartbeat thread main loop
 */
void hbThreadMain(void) {
    unsigned int timer = 0;
    heartbeatLED = &normalLED;
    
    Sensor *s = sensors->getSensor("odo");
    Sensor *s2 = sensors->getSensor("sonFront");
    
    while (true) {
        *heartbeatLED = 0;  // on
        Thread::wait(100);
        *heartbeatLED = 1;  // off
        Thread::wait(1900);
        
        if(timer % 30 == 0) // once in every 60 seconds
            checkBattery();
//        if(timer % 5 == 0) // once in every 10 seconds
//            printf("Odometry total distance: %f %s\n", s->readValue(), s->getMetric());
        if(timer % 1 == 0) { // once in every 2 seconds
            //sensors->convertTemperature(false); // TODO: improve DS18B20 lib robustness, then uncomment
        }
        timer++;
    }
}

/**
 * Telemetry thread main loop
 */
void tmThreadMain(void) {
    Sensor *odometry = sensors->getSensor("odo");
    Sensor *frontSonar = sensors->getSensor("sonFront");

    // 10 Hz update loop
    while(true) {
        float speed = odometry->readValue(Odometry::CurrentSpeed);
        if(speed != 0)
            printf("Speed: %.1f %s", speed, odometry->getMetric(Odometry::CurrentSpeed));
            
        float clearance = frontSonar->readValue();
        if(clearance < 90) {
            if(speed != 0) {
                printf(" | Front sonar clearance: %.1f %s", clearance, frontSonar->getMetric());
            }
        }
        
        if((speed != 0))//||(clearance < 90))
            printf("\n");
        
        Thread::wait(100);
    }
}

int main() {
    pc.baud(115200);
    printf("OS Started\n");

    // Init peripherals
    
    // LEDs have inverse logic
    normalLED = 1;
    errorLED = 1;
    cmdExecLED = 1;

    BT.baud(9600); // HC-05 module works at this rate by default
    BT.attach(&gotChar); // register interrupt handler
    
    btConnTimeout.attach(&onBTtimeout, 2);

    // index bal: PTA1 / D3
    // index jobb: PTB9 / D2
    // nappali fény: PTC3 / D7
    // reflektor: PTC2 / D6
    // tolatólámpa: PTB23 / D4
    // féklámpa: PTA2 / D5
    lights = new Lights(PTA1, PTB9, PTC3, PTC2, PTB23, PTA2);

    // sensor objects can be obtained from this object
    sensors = new Sensors();
    
    // előre: PTD0 / D10
    // hátra: PTD2 / D11
    // kormány: PTC4 / D9
    // passing lights, thus turning on automatic index and reversing lights
    // passing sensors to make odometric feedback and sonar based autobraking possible
    drive = new Drive(PTD0, PTD2, PTC4, lights, sensors);

    // simple demo of autonomous operation (currently sensorless, timing based)
    demo = new Demo(drive, lights);

    printf("Starting threads\n");
    
    hbThread.start(callback(hbThreadMain));
    cmdHandlerThread.start(callback(cmdHandlerMain));
    tmThread.start(callback(tmThreadMain));
    
    // main control loop
    while(1) {
        // Currently not doing anything here        
        Thread::wait(2000);
    }
    printf("End of main (never should be executed)\n");
}
