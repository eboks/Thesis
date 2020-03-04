#ifndef CONFIG_H
#define CONFIG_H

// general settings
#define FFT_ENABLE  //enable to run fft
#define DEMPING    //energy mode

//fft configurations
#define SAMPLES 4096            //Must be a power of 2
#define SAMPLING_FREQUENCY 200000  //total sampling time = SAMPLES/SAMPLING_FREQUENCY
#define AVERAGE 1                   //total averaging
#define AMOUNT_ARRAY 7              //amount of outputs for classification
#define TIME_BETWEEN_FFT 30       //amount of miliseconds between each fft
#define KNOPDEBOUNCE 1100           //minimum amount of time between button pushes
#define AMOUNT_SAME 2               //the amount of times that the outcome has to be te same before changing the output
#define MINIMUM_CORRELATION 0.7     //the minimum amount of correlation for a sample to be valid
#define AVERAGE_SAMPLES 10           //The amount of samples that average to 1 ideal example sample
#define SAMPLES_PER_OUTPUT 3         // The amount of ideal samples for each output
#define MINFREQ 25000         // min freq for corr
#define MAXFREQ 50000         //max freq for corr
#define MIN_AMPLITUDE 8000    //min amplitude to be valid
#define AMOUNT_FREQ_BINS (MAXFREQ-MINFREQ)*SAMPLES/SAMPLING_FREQUENCY  //the amount of frequency 
#define AMOUNT_MOVING_AVG 5


// I/O pins declarations
#define LEDPIN 13  //define the onboard LEDPIN
#define ANALOG3 17  //define analog read 9 pin
#define CONTROL 4  //CTRL pin for waveform IC
#define CS1 6     //CS pin of SPI
#define SAMPLE 8  //pin for taking samples
#define CESWEEP 16     //CE pin of RF
#define CSNSWEEP 15     //CSN pin of 
#define CESENSE 10     //CE pin of RF
#define CSNSENSE 16     //CSN pin of RF
#define BUTTONINDICATOR 0  //indicator LED
#define TRISTATE 23
#define SYNCOUT 22
#define INTERRUPT 3
#define LEDPIN2 22
#endif