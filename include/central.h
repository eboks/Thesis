#ifndef CENTRAL_H
#define CENTRAL_H

#include <Arduino.h>
#include "config.h"
#include <nRF24L01.h>
#include <RF24.h>

class central{

// setup the central
public: void setup();

// Run the central
public: void run();

};
#endif 