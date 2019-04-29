#include "sonar.h"

void Sonar::init() {
    trigger = 0;
    distance = -1;

    echo.rise(callback(this, &Sonar::echo_in));    // Attach handler to the rising interruptIn edge
    echo.fall(callback(this, &Sonar::echo_fall));  // Attach handler to the falling interruptIn edge
}

/**
 *  Start the background task to trigger sonar reads every 100ms
 */
void Sonar::start(void) {
    ticker.attach(callback(this, &Sonar::background_read), 0.1f);
}

/**
 *  Stop the background task that triggers sonar reads
 */
void Sonar::stop(void) {
    ticker.detach();
}

/**
 *  Interrupt pin rising edge interrupt handler. Reset and start timer
 */
void Sonar::echo_in(void) {
       timer.reset();
       timer.start();
       begin = timer.read_us();
}

/**
 *  Interrupt pin falling edge interrupt handler. Read and disengage timer.
 *  Calculate raw echo pulse length
 */
void Sonar::echo_fall(void) {
    end = timer.read_us();
    timer.stop();
    distance = end - begin;
}

/**
 *  Wrapper function to set the trigger pin low. Callbacks cannot take in both object and argument pointers.
 *  See use of this function in background_read().
 */
void Sonar::trigger_toggle(void) {
    trigger = 0;
}

/**
 *  Background callback task attached to the periodic ticker that kicks off sonar reads
 */
void Sonar::background_read(void) {
    trigger = 1;
    timeout.attach_us(callback(this, &Sonar::trigger_toggle), 10);
}

/**
 *  Public read function that returns the scaled distance result in cm
 */
float Sonar::readValue(unsigned int readingId) {
       return distance / 58.0f;
}
