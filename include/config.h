#ifndef CONFIG_H
#define CONFIG_H

/*
    All the settings the user is able to adjust
*/

// general settings
#define FFT_ENABLE true //enable to run fft
#define PROGRAM_ENABLE false // enable programming IC mode

//fft configurations
#define SAMPLES 4096            //Must be a power of 2
#define SAMPLING_FREQUENCY 200000  //total sampling time = SAMPLES/SAMPLING_FREQUENCY
#define AVERAGE 1                   //total amount of averaging

// I/O pins declarations
#define LEDPIN 13  //define the onboard LEDPIN
#define ANALOG9 23  //define analog read 9 pin
#define ANALOG0 14  //define analog read 0 pin
#define BUTTONPIN 16  //define A2/16 as buttonpin
#define CONTROL 2  //CTRL pin for waveform IC
#define CS 10     //CS pin of SPI
#define ENDSWEEP 1     //pin that reads the end of the sweep
#define STOP 0  

#endif