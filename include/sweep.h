#ifndef SWEEP_H
#define SWEEP_H

#include <Arduino.h>
#include "config.h"
#include <nRF24L01.h>
#include <RF24.h>

class sweep{

// setup the sweep
public: void setup();

// Run the sweep
public: void run();

// put enable pin low and transfer data via spi
private: void spitransfer(int code);

};
#endif 