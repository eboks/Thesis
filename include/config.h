#ifndef CONFIG_H
#define CONFIG_H

// general settings
//#define SWEEP  //Program sweep module
#define SENSE  //Program sense module
//#define CENTRAL  //Program central module
//#define DEMPING    //energy mode
//#define OLD
#define NEW

//node program settings
#define SENSENODE 01          //sense nodes are 01, 02, 03, ...
#define SWEEPNODE 011          //sweep nodes are 011, 021, 031

//fft configurations
#define SAMPLES 4096            //Must be a power of 2
#define SAMPLING_FREQUENCY 200000  //total sampling time = SAMPLES/SAMPLING_FREQUENCY
#define AVERAGE 1                   //total averaging
#define AMOUNT_ARRAY 7              //amount of outputs for classification
#define TIME_BETWEEN_FFT 50       //amount of miliseconds between each fft
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

//general I/O pins declarations
#define ONBOARDLEDPIN 13 //Define the onboardledpin (also the csk of SPI)

#ifdef NEW
// I/O pins declarations for sweep
#define CONTROL 4  //CTRL pin for waveform IC
#define CS1 8     //CS pin of SPI
#define CESWEEP 14     //CE pin of RF
#define CSNSWEEP 15     //CSN pin of 
#define TRISTATE 21         //has to be high for the sweep oscillator to be on
#define SYNCOUT 3          //needed to end the sweep
#define INTERRUPT 5         //needed to end the sweep
#endif

#ifdef OLD
// I/O pins declarations for sweep
#define CONTROL 4  //CTRL pin for waveform IC
#define CS1 6     //CS pin of SPI
#define CESWEEP 16     //CE pin of RF
#define CSNSWEEP 15     //CSN pin of 
#define TRISTATE 23         //has to be high for the sweep oscillator to be on
#define SYNCOUT 22          //needed to end the sweep
#define INTERRUPT 3         //needed to end the sweep
#endif

#ifdef NEW
// I/O pins declarations for sense
#define CESENSE 10     //CE pin of RF
#define CSNSENSE 16     //CSN pin of RF
#define BUTTONINDICATOR 0  //indicator LED
#define ANALOG3 17  //define analog read 9 pin
#define SAMPLE 4  //input pin for button to make a sample
#define LEDPIN 5  //define the pin we use as output for the damping
#endif

#ifdef OLD
// I/O pins declarations for sense
#define CESENSE 10     //CE pin of RF
#define CSNSENSE 16     //CSN pin of RF
#define BUTTONINDICATOR 0  //indicator LED
#define ANALOG3 17  //define analog read 9 pin
#define SAMPLE 8  //input pin for button to make a sample
#define LEDPIN 22  //define the pin we use as output for the damping
#endif

// I/O pins declarations for central


#endif