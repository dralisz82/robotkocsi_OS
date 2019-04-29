#include "odometry.h"

void Odometry::init() {
    hallA = new InterruptIn(PTC16); // D0
    hallB = new InterruptIn(PTC17); // D1
    odoCount = 0;
    lastHall = X;
    dir = FW;
    hallA->rise(callback(this, &Odometry::hallAISR));
    hallB->rise(callback(this, &Odometry::hallBISR));

}

void Odometry::hallAISR(void) {
    if(lastHall == A)
        toggleDir();

    if(dir == FW)
        odoCount++;
    else
        odoCount--;

    lastHall = A;
}

void Odometry::hallBISR(void) {
    if(lastHall == B)
        toggleDir();

    if(dir == FW)
        odoCount++;
    else
        odoCount--;

    lastHall = B;
}

void Odometry::toggleDir() {
    if(dir == FW)
        dir = BW;
    else
        dir = FW;
}

float Odometry::readValue() {
    return odoCount * (WHEELCIRCUMFERENCE / PPR) / 10.0;   // Total distance taken since startup
}
