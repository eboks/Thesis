#ifndef TESTCODE_H
#define TESTCODE_H

#include <Arduino.h>

// pinout
#define LEDPIN 13  //define the onboard LEDPIN
#define ANALOG9 23  //define analog read 9 pin

class testcode{

// Initialise the pins used by the LEDs as output
public: void init();

// Test the sampling rate of the uC
public: void testsamplingrate();

};
#endif 