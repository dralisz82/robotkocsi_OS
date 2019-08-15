#include "odometry.h"

void Odometry::init() {
    hallA = new InterruptIn(PTC16); // D0
    hallB = new InterruptIn(PTC17); // D1
    timer.start();
    hallTimeout.attach(callback(this, &Odometry::clearSpeed), 1);
    dir = X;
    hallA->rise(callback(this, &Odometry::hallAriseISR));
    hallA->fall(callback(this, &Odometry::hallAfallISR));
    hallB->rise(callback(this, &Odometry::hallBriseISR));
    hallB->fall(callback(this, &Odometry::hallBfallISR));

    previousTime = timer.read_ms();
    previousSpeed = 0;
    speedSum = 0;
    odoCount = 0;
    odoPos = 0;
    currentSpeed = 0;
    avgSpeed = 0;
    highestSpeed = 0;
}

void Odometry::hallAriseISR(void) {
    if(hallB->read() == 0) {
        // 00 -> 10
        dir = BW;
    } else {
        // 01 -> 11
        dir = FW;
    }

    if(dir == FW)
        odoPos++;
    else
        odoPos--;

    odoCount++;
    calcSpeed();
}

void Odometry::hallAfallISR(void) {
    if(hallB->read() == 0) {
        // 10 -> 00
        dir = FW;
    } else {
        // 11 -> 01
        dir = BW;
    }

    if(dir == FW)
        odoPos++;
    else
        odoPos--;

    odoCount++;
    calcSpeed();
}

void Odometry::hallBriseISR(void) {
    if(hallA->read() == 0) {
        // 00 -> 01
        dir = FW;
    } else {
        // 10 -> 11
        dir = BW;
    }

    if(dir == FW)
        odoPos++;
    else
        odoPos--;

    odoCount++;
    calcSpeed();
}

void Odometry::hallBfallISR(void) {
    if(hallA->read() == 0) {
        // 01 -> 00
        dir = BW;
    } else {
        // 11 -> 10
        dir = FW;
    }

    if(dir == FW)
        odoPos++;
    else
        odoPos--;

    odoCount++;
    calcSpeed();
}

void Odometry::calcSpeed() {
    // Prevent division by zero -> side effect: measuring limit under 833 RPM (@72 PPR)
    if(timer.read_ms() == previousTime)
        return;
    
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

    hallTimeout.detach();
    hallTimeout.attach(callback(this, &Odometry::clearSpeed), 1);
}

void Odometry::clearSpeed() {
    currentSpeed = 0;
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
        case CurrentSpeed:      return currentSpeed * ((dir == BW)?-1:1);
        case AvgSpeed:          return avgSpeed;
        case HighestSpeed:      return highestSpeed;
    }
    return odoCount * (WHEELCIRCUMFERENCE / PPR) / 10.0;   // Total distance taken since startup
}
