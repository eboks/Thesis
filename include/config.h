#ifndef CONFIG_H
#define CONFIG_H

/*
    All the settings the user is able to adjust
*/

// general settings

//fft configurations
#define SAMPLES 4096            //Must be a power of 2
#define SAMPLING_FREQUENCY 200000  //total sampling time = SAMPLES/SAMPLING_FREQUENCY

// I/O pins declarations
#define LEDPIN 13  //define the onboard LEDPIN
#define ANALOG9 23  //define analog read 9 pin
#define ANALOG0 14  //define analog read 0 pin
#define BUTTONPIN 16  //define A2/16 as buttonpin

#endif