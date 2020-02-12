#ifndef FFT_H
#define FFT_H

#include <Arduino.h>
#include "config.h"
#include <arduinoFFT.h>

class fft{

// Run the fft, once if state = 0, continiously if state = 1
public: void runfft(byte state);

// Calculate the actual fft
private: void dofft();

// Print the results to plot it in serialplot
private: void fftPlot();

};
#endif 