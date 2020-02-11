#include <Arduino.h>
#include <arduinoFFT.h>

#include "config.h"
#include "main.h"
#include "testcode.h"
#include "Plotter.h"

testcode test;

#define SAMPLES 4096            //Must be a power of 2
#define SAMPLING_FREQUENCY 200000 //Hz, must be less than 10000 due to ADC
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;
 
double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);       // baud rate is ignored with Teensy USB ACM i/o
  pinMode(LEDPIN, OUTPUT);  

  pinMode(ANALOG0, INPUT);
  analogReadRes(10);          // set ADC resolution to this many bits
  analogReadAveraging(1);    // average this many readings
  
  digitalWrite(LEDPIN,HIGH);   
  delay(1000);   // LED on for 1 second
  digitalWrite(LEDPIN,LOW);    
  delay(3000);   // wait in case serial monitor still opening

  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));  //for the sampling frequency

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(ANALOG0);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){ //To set sampling frequency
        }
    }
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
    /*PRINT RESULTS*/
    Serial.println(peak);     //Print out what frequency is the most dominant.
 
    for(int i=0; i<(SAMPLES/2); i++)
    {
        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
         
        Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
        Serial.print(": ");
        Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins, 1 is 1 number after the colon
    }
 
    //delay(1000);  //Repeat the process every second OR:
    while(1);       //Run code once

}