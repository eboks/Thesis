#ifndef CONFIG_H
#define CONFIG_H

// general settings
#define FFT_ENABLE  //enable to run fft

//fft configurations
#define SAMPLES 4096            //Must be a power of 2
#define SAMPLING_FREQUENCY 200000  //total sampling time = SAMPLES/SAMPLING_FREQUENCY
#define AVERAGE 10                   //total averaging

// I/O pins declarations
#define LEDPIN 13  //define the onboard LEDPIN
#define ANALOG9 23  //define analog read 9 pin
#define CONTROL 2  //CTRL pin for waveform IC
#define CS1 10     //CS pin of SPI
#define VERZENDCONTROL 7  //pin to forward the control  

#endif