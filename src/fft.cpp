#include "fft.h"

arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned int knoptiming;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];
//double vavg[SAMPLES];

double samples[410][AMOUNT_ARRAY];
int counter = 0;
int minimum = 0;
int vorige = 0;
int output = 0;
int plotinterval = 0;
int aantalkeren=0;

unsigned int tijd = 0;

RF24 radio(CE, CSN); // CE, CSN
const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
char startsweep[10] = "hey";
void fft::runfft(byte state) //SWEEP DUURT 20.48 ms!!!!
{
  //RF SETTINGS
  radio.begin(); //Starting the Wireless communication
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(10);
  radio.setRetries(3, 10);             // delay, count
  radio.openWritingPipe(slaveAddress); //Setting the address where we will send the data
  radio.stopListening();               //This sets the module as transmitter

  for (int j = 0; j < AMOUNT_ARRAY; j++)
  {
    for (int i = 0; i < 410; i++)
    {
      samples[i][j]=1000000;
    }
  }

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY)); //for the sampling frequency
  while (1)
  {
    if (tijd + TIME_BETWEEN_FFT < millis())
    { //update every ... seconds
      tijd = millis();
      boolean succes = radio.write(&startsweep, sizeof(startsweep));
      if (succes == true)
      {
        dofft();
        updateLED();
        if (plotinterval >= 25)
        {
          plotinterval = 0;
          fftPlot();
        }
      }
      plotinterval++;
    }
  }
}

void fft::dofft()
{
  for (int i = 0; i < SAMPLES; i++)
  {
    microseconds = micros(); //Overflows after around 70 minutes!

    vReal[i] = analogRead(ANALOG9);
    vImag[i] = 0;
    while (micros() < (microseconds + sampling_period_us))
    { //To set sampling frequency
    }
  }
  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
}

void fft::fftPlot()
{
  for (int i = 0; i < (SAMPLES / 2); i++)
  {
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
    double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
    if (frequency < 41000)
    {                              //only plot the usefull range
      Serial.println(vReal[i], 1); //View only this line in serial plotter to visualize the bins, 1 is 1 number after the colon
    }
  }
}

void fft::updateLED()
{
  if (digitalRead(SAMPLE) && millis() > knoptiming + KNOPDEBOUNCE)
  {
    knoptiming = millis();
    int hoeveelste = 0;
    for (int i = 0; i < (SAMPLES / 2); i++)
    {
      double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
      if (frequency > 20000 && frequency < 40000)
      { //410 frequency bins!!
        samples[hoeveelste][counter] = vReal[i];
        hoeveelste++;
      }
    }
    counter++;
    if (counter > AMOUNT_ARRAY - 1)
    {
      counter = 0;
    }
  }
  else
  {
    double correlatie[AMOUNT_ARRAY];
    for (int k = 0; k < AMOUNT_ARRAY; k++)
    {
      correlatie[k] = 0;
      int hoeveelste = 0;
      double teller=0;
      double noemer1=0;
      double noemer2=0;
      for (int i = 0; i < (SAMPLES / 2); i++)
      {
        double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
        if (frequency > 20000 && frequency < 40000)
        { //410 frequency bins!!
          teller += samples[hoeveelste][k]*vReal[i];
          noemer1 += samples[hoeveelste][k]*samples[hoeveelste][k];
          noemer2 += vReal[i]*vReal[i];
          hoeveelste++;
        }
      }
      correlatie[k]= teller/sqrt(noemer1*noemer2);
    } 
    digitalWrite(output + 16, LOW);
    minimum = 0;
    for (int k = 0; k < AMOUNT_ARRAY; k++)
    {
      if (correlatie[k] > correlatie[minimum])
      {
        minimum = k;
      }
    }
    if(minimum==vorige){
      aantalkeren++;
    }
    else{
      aantalkeren = 0;
      vorige = minimum;
    }
    if(aantalkeren==AMOUNT_SAME){
      output = minimum;
      aantalkeren = 0;
    }
    digitalWrite(output + 16, HIGH);
  }
}