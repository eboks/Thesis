#ifndef CONFIG_H
#define CONFIG_H

/*
    All the settings the user is able to adjust
*/

// general settings
#define FFT_ENABLE false //enable to run fft
#define PROGRAM_ENABLE true // enable programming IC mode

//fft configurations
#define SAMPLES 4096            //Must be a power of 2
#define SAMPLING_FREQUENCY 200000  //total sampling time = SAMPLES/SAMPLING_FREQUENCY
#define AVERAGE 1                   //total averaging

// I/O pins declarations
#define LEDPIN 13  //define the onboard LEDPIN
#define ANALOG9 23  //define analog read 9 pin
#define ANALOG0 14  //define analog read 0 pin
#define BUTTONPIN 16  //define A2/16 as buttonpin
#define CONTROL 2  //CTRL pin for waveform IC
#define CS1 10     //CS pin of SPI
#define CS2 16     //CS pin of SPI
#define ENDSWEEP 1     //pin that reads the end of the sweep
#define STOP 0  
#define VERZENDCONTROL 7  //pin to forward the control  
#define ONTVANGCONTROL 17  //pin to forward receive the control  


#endif