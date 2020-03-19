#ifndef SENSE_H
#define SENSE_H

#include <Arduino.h>
#include "config.h"
#include <arduinoFFT.h>
#include <nRF24L01.h>
#include <RF24.h>

class sense{

// setup the sense
public: void setup();

// Run the fft
public: void run();

// Calculate the actual fft
private: void dofft();

// Print the results to plot it in serialplot
private: void fftPlot();

// update the output with energy
private: void updateEnLED();

// update the output with correlation
private: void updateCorLED();

// calculate the average energy
private: void averageEnergy();

};
#endif 