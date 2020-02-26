#ifndef FFT_H
#define FFT_H

#include <Arduino.h>
#include "config.h"
#include <arduinoFFT.h>
#include <nRF24L01.h>
#include <RF24.h>

class fft{

// Run the fft, once if state = 0, continiously if state = 1
public: void runfft(byte state);

// Calculate the actual fft
private: void dofft();

// Print the results to plot it in serialplot
private: void fftPlot();

// update the output
private: void updateLED();
};
#endif 