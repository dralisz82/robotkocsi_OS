#include "odometry.h"

void Odometry::init() {
    hallA = new InterruptIn(PTC16); // D0
    hallB = new InterruptIn(PTC17); // D1
    timer.start();
    lastHall = X;
    dir = FW;
    hallA->rise(callback(this, &Odometry::hallAISR));
    hallB->rise(callback(this, &Odometry::hallBISR));

    previousTime = timer.read_ms();
    previousSpeed = 0;
    speedSum = 0;
    odoCount = 0;
    odoPos = 0;
    currentSpeed = 0;
    avgSpeed = 0;
    highestSpeed = 0;
}

void Odometry::hallAISR(void) {
    if(lastHall == A)
        toggleDir();

    if(dir == FW)
        odoPos++;
    else
        odoPos--;

    odoCount++;
    calcSpeed(A);

    lastHall = A;
}

void Odometry::hallBISR(void) {
    if(lastHall == B)
        toggleDir();

    if(dir == FW)
        odoPos++;
    else
        odoPos--;

    odoCount++;
    calcSpeed(B);

    lastHall = B;
}

void Odometry::calcSpeed(HallID actHall) {
    // Prevent division by zero -> side effect: measuring limit under 5000 RPM (@12 PPR)
    if(timer.read_ms() == previousTime)
        return;
    
    // Handle direction change
    if(actHall == lastHall) {
        currentSpeed = 0;
        return;
    }
    
    // Current speed calculation
    currentSpeed = (WHEELCIRCUMFERENCE / PPR) * 100 / (timer.read_ms() - previousTime);

    // Highest speed calculation
    if((previousSpeed + currentSpeed)/2 > highestSpeed)
        highestSpeed = (previousSpeed + currentSpeed)/2;
    
    // Average speed calculation
    speedSum += currentSpeed;   // TODO: this will overflow once, far in the future
    avgSpeed = speedSum / odoCount;

    previousTime = timer.read_ms();
    previousSpeed = currentSpeed;
}


void Odometry::toggleDir() {
    if(dir == FW)
        dir = BW;
    else
        dir = FW;
}

/// Public functions

unsigned int Odometry::getNumberOfReadings() {
    return 5;
}

char* Odometry::getReadingName(unsigned int readingId) {
    switch(readingId) {
        case Distance:          return "distance";
        case RelativePosition:  return "relative position";
        case CurrentSpeed:      return "speed";
        case AvgSpeed:          return "average speed";
        case HighestSpeed:      return "highest speed";
    }
    return name;
}

char* Odometry::getMetric(unsigned int readingId) {
    switch(readingId) {
        case Distance:          return "cm";
        case RelativePosition:  return "cm";
        case CurrentSpeed:      return "cm/s";
        case AvgSpeed:          return "cm/s";
        case HighestSpeed:      return "cm/s";
    }
    return metric;
}

float Odometry::readValue(unsigned int readingId) {
    switch(readingId) {
        case Distance:          return odoCount * (WHEELCIRCUMFERENCE / PPR) / 10.0;
        case RelativePosition:  return odoPos * (WHEELCIRCUMFERENCE / PPR) / 10.0;
        case CurrentSpeed:      return currentSpeed * (dir == FW)?1:-1;
        case AvgSpeed:          return avgSpeed;
        case HighestSpeed:      return highestSpeed;
    }
    return odoCount * (WHEELCIRCUMFERENCE / PPR) / 10.0;   // Total distance taken since startup
}
