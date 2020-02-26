#ifndef CONFIG_H
#define CONFIG_H

// general settings
#define FFT_ENABLE  //enable to run fft

//fft configurations
#define SAMPLES 4096            //Must be a power of 2
#define SAMPLING_FREQUENCY 200000  //total sampling time = SAMPLES/SAMPLING_FREQUENCY
#define AVERAGE 1                   //total averaging
#define AMOUNT_ARRAY 6              //amount of outputs for classification
#define TIME_BETWEEN_FFT 30       //amount of miliseconds between each fft
#define KNOPDEBOUNCE 3000           //minimum amount of time between button pushes
#define AMOUNT_SAME 1               //the amount of times that the outcome has to be te same before changing the output

// I/O pins declarations
#define LEDPIN 13  //define the onboard LEDPIN
#define ANALOG9 23  //define analog read 9 pin
#define CONTROL 2  //CTRL pin for waveform IC
#define CS1 10     //CS pin of SPI
#define SAMPLE 14  //pin for taking samples
#define CE 8     //CE pin of RF
#define CSN 9     //CSN pin of RF



#endif