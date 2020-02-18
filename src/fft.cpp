#include "fft.h"

arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];
double vavg[SAMPLES];

int counter = 0;

unsigned int tijd = 0;

void fft::runfft(byte state) //SWEEP DUURT 20.48 ms!!!!
{
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY)); //for the sampling frequency
  while (1)
  {
    if (tijd + 1000 < millis())
    { //update every 2 seconds
      for (int i = 0; i < (SAMPLES / 2); i++)
      {
        vavg[i] = 0;
      }
      for (int i = 0; i < 10; i++)
      { //average over 10 times
        dofft();
        for (int j = 0; j < (SAMPLES / 2); j++)
        {
          vavg[j] += vReal[j];
        }
      }
      for (int i = 0; i < (SAMPLES / 2); i++)
      {
        vReal[i] = vavg[i] / 10;
      }
      tijd = millis();
      fftPlot();
    }
  }
}

void fft::dofft()
{
  //digitalWrite(CONTROL,HIGH);   //start sweep
  for (int i = 0; i < SAMPLES; i++)
  {
    microseconds = micros(); //Overflows after around 70 minutes!

    vReal[i] = analogRead(ANALOG9);
    vImag[i] = 0;

    while (micros() < (microseconds + sampling_period_us))
    { //To set sampling frequency
    }
  }
  //digitalWrite(CONTROL,LOW);   //start sweep
  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  //double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  /*PRINT RESULTS*/
  //Serial.println(peak);     //Print out what frequency is the most dominant.

  //delay(1000);  //Repeat the process every second OR:
  //while(1);       //Run code once
}

void fft::fftPlot()
{
  for (int i = 0; i < (SAMPLES / 2); i++)
  {
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
    double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
    if (frequency < 41000)
    { //only plot the usefull range
      //Serial.print(frequency, 1);
      //Serial.print(": ");
      Serial.println(vReal[i], 1); //View only this line in serial plotter to visualize the bins, 1 is 1 number after the colon
      //counter++;  //count how many usefull bins
    }
  }
  //Serial.print("counter: ");
  //Serial.print(counter);
}