#include "mbed.h"
#include "rtos.h"
#include "commandhandler.h"
#include "robotkocsi/drive.h"
#include "robotkocsi/lights.h"
#include "robotkocsi/sensors.h"
#include "robotkocsi/demo.h"


DigitalOut heartbeatLED(LED_GREEN);
DigitalOut cmdExecLED(LED_RED);

Lights* lights;
Drive* drive;
Sensors* sensors;
Demo* demo;

Serial BT(PTC15, PTC14);  // Bluetooth module header

Thread *hbThread;  // heartbeat thread
//Thread *nw_thread;  // responsible for ethernet network
extern Thread* cmdHandlerThread;

void execCommand(char *cmd, int argc, simplestr *args) {
    if(!strcmp(cmd, "forward")) {
        drive->forward();// lights->reversingLightOff();
    }
    else if(!strcmp(cmd, "backward")) {
        drive->backward();// lights->reversingLightOn();
    }
    else if(!strcmp(cmd, "halt")) {
        drive->stop();// lights->reversingLightOff();
    }
    else if(!strcmp(cmd, "left")) {
        drive->steerLeft();// lights->indexLeft();
    }
    else if(!strcmp(cmd, "right")) {
        drive->steerRight();// lights->indexRight();
    }
    else if(!strcmp(cmd, "straight")) {
        drive->steerStraight();// lights->indexOff();
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
        if(s != NULL) {
            BT.printf("%s: %f %s\n", s->getName(), s->readValue(), s->getMetric());
        }
    }
    else {
        lights->hazardLightsOn();  // ignore invalid command, put emergency indicator on
    }
    printf("%s %d; %s, %s, %s, %s\n", cmd, argc, args[0], args[1], args[2], args[3]);
}

/**
 * Heartbeat thread main loop
 */
void hbThreadMain(void const *argument) {
    while (true) {
        heartbeatLED = 0;
        Thread::wait(100);
        heartbeatLED = 1;
        Thread::wait(1900);
    }
}


/**
 * Network thread main loop
 */
/*void nw_thread_main(void const *argument) {
    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();
    printf("IP Address is %s\n", eth.getIPAddress());
    
    TCPSocketConnection sock;
    sock.connect("mbed.org", 80);
    
    char http_cmd[] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\n\n";
    sock.send_all(http_cmd, sizeof(http_cmd)-1);
    
    char buffer[300];
    int ret;
    while (true) {
        ret = sock.receive(buffer, sizeof(buffer)-1);
        if (ret <= 0)
            break;
        buffer[ret] = '\0';
        printf("Received %d chars from server:\n%s\n", ret, buffer);
    }
      
    sock.close();
    
    eth.disconnect();
}*/

int main() {
    printf("OS Started\n");

    // Init peripherals
    heartbeatLED = 1;
    cmdExecLED = 1;

    BT.baud(9600); // HC-05 module works at this rate by default
    BT.attach(&gotChar); // register interrupt handler

    // index bal: PTA1 / D3
    // index jobb: PTB9 / D2
    // nappali fény: PTC3 / D7
    // reflektor: PTC2 / D6
    // tolatólámpa: PTB23 / D4
    // féklámpa: PTA2 / D5
    lights = new Lights(PTA1, PTB9, PTC3, PTC2, PTB23, PTA2);

    // előre: PTD0 / D10
    // hátra: PTD1 / D13
    // balra: PTD2 / D11
    // jobbra: PTD3 / D12
    // passing lights, thus turning on automatic index and reversing lights
    drive = new Drive(PTD0, PTD1, PTD2, PTD3, lights);

    // simple demo of autonomous operation (currently sensorless, timing based)
    demo = new Demo(drive, lights);

    // sensor objects can be obtained from this object
    sensors = new Sensors();
    
    printf("Starting threads\n");
    
    hbThread = new Thread(hbThreadMain);
//    nw_thread = new Thread(nw_thread_main);
    cmdHandlerThread = new Thread(cmdHandlerMain);
    
    // main control loop
    while(1) {
        // Currently not doing anything here        
        Thread::wait(2000);
    }
    printf("End of main (never should be executed)\n");
}