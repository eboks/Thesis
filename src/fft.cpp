#include "fft.h"

arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned int knoptiming;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];
double vavg[SAMPLES];

double samples[AMOUNT_FREQ_BINS][AMOUNT_ARRAY][SAMPLES_PER_OUTPUT];
int counter = 0;
int counter2 = 0;
int minimum1 = 0;
int minimum2 = 0;
int vorige = 0;
int output = 0;
int plotinterval = 0;
int aantalkeren = 0;
double totalenergy = 0;
double demping = 0;
double PWM_value = 255;
double moving_avg[AMOUNT_MOVING_AVG];
int moving_pos = 0;
double totaal = 0;

int teller = 0;

boolean takesample = false;
int sample_number = 0;

unsigned int tijd = 0;

RF24 radio(CESENSE, CSNSENSE); // CE, CSN
const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
char startsweep[10] = "hey";
void fft::runfft(byte state) //SWEEP DUURT 20.48 ms!!!!
{
  //RF SETTINGS
  radio.begin(); //Starting the Wireless communication
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(10);
  radio.setRetries(3, 10);             // delay, count
  radio.openWritingPipe(slaveAddress); //Setting the address where we will send the data
  radio.stopListening();               //This sets the module as transmitter

  for (int j = 0; j < AMOUNT_ARRAY; j++)
  {
    for (int i = 0; i < AMOUNT_FREQ_BINS; i++)
    {
      for (int k = 0; k < SAMPLES_PER_OUTPUT; k++)
      {
        samples[i][j][k] = -1000000;
      }
    }
  }

  for (int i = 0; i < AMOUNT_MOVING_AVG; i++)
  {
    moving_avg[i] = 0;
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
        if (plotinterval >= 25) //plot the values once in 25 times;
        {
          plotinterval = 0;
          fftPlot();/*
          averageEnergy();
          double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
          Serial.print("piek: ");
          Serial.println(peak);
          int bin = (peak*SAMPLES)/SAMPLING_FREQUENCY;
          Serial.print("sterkte: ");
          Serial.println(vReal[bin]);*/
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
    vReal[i] = analogRead(ANALOG3);
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
  //teller = 0;
  for (int i = 0; i < (SAMPLES / 2); i++)
  {
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
    double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
    if (frequency < 80000) //840 samples bij 41000 || 1639  bij 80000
    {                      //only plot the usefull range
      //Serial.print(i);
      //Serial.print(": ");
      Serial.println(vReal[i], 1); //View only this line in serial plotter to visualize the bins, 1 is 1 number after the colon
      //teller++;
    }
  }
  //Serial.println(teller);
}

void fft::updateLED()
{
  /*for (int i = 0; i < (SAMPLES / 2); i++)
  {
    vReal[i] = 20 * log10(vReal[i] / 1500);
  }*/
  if (millis() > knoptiming + KNOPDEBOUNCE)
  {
    digitalWrite(BUTTONINDICATOR, LOW);
  }
  if (digitalRead(SAMPLE) && (millis() > knoptiming + KNOPDEBOUNCE) && takesample == false)
  {
    digitalWrite(BUTTONINDICATOR, HIGH);
    takesample = true;
    sample_number = 0;
  }
  if (takesample)
  {
    knoptiming = millis();
#ifndef DEMPING
    for (int i = 0; i < (SAMPLES / 2); i++)
    {
      double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
      if (frequency > MINFREQ && frequency < MAXFREQ)
      { //410 frequency bins!!
        vavg[i] += vReal[i];
      }
    }
    if (sample_number >= AVERAGE_SAMPLES)
    {
      int hoeveelste = 0;
      for (int i = 0; i < (SAMPLES / 2); i++)
      {
        double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
        if (frequency > MINFREQ && frequency < MAXFREQ)
        { //410 frequency bins!!
          samples[hoeveelste][counter][counter2] = vavg[i] / AVERAGE_SAMPLES;
          hoeveelste++;
        }
      }
      takesample = false;
      counter2++;
      if (counter2 >= SAMPLES_PER_OUTPUT)
      {
        counter++;
        counter2 = 0;
      }
      if (counter >= AMOUNT_ARRAY)
      {
        counter = 0;
      }
    }
    sample_number++;
#endif

#ifdef DEMPING
    int hoeveelste = 0;
    totalenergy = 0;
    for (int i = 0; i < (SAMPLES / 2); i++)
    {
      double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
      if (frequency > MINFREQ && frequency < MAXFREQ)
      { //410 frequency bins!!
        samples[hoeveelste][0][0] = vReal[i];
        //if (vReal[i] >= MIN_AMPLITUDE)
        //{
        totalenergy += vReal[i];
        // }
        hoeveelste++;
      }
    }
    takesample = false;
#endif
  }
  else
  {
#ifndef DEMPING
    double correlatie[AMOUNT_ARRAY][SAMPLES_PER_OUTPUT];
    for (int k = 0; k < AMOUNT_ARRAY; k++)
    {
      for (int j = 0; j < SAMPLES_PER_OUTPUT; j++)
      {
        correlatie[k][j] = 0;
        int hoeveelste = 0;
        double teller = 0;
        double noemer1 = 0;
        double noemer2 = 0;
        for (int i = 0; i < (SAMPLES / 2); i++)
        {
          double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
          if (frequency > MINFREQ && frequency < MAXFREQ)
          { //410 frequency bins bij 20-40kHz!!
            if (samples[hoeveelste][0][0] >= MIN_AMPLITUDE)
            {
              teller += samples[hoeveelste][k][j] * vReal[i];
              noemer1 += samples[hoeveelste][k][j] * samples[hoeveelste][k][j];
              noemer2 += vReal[i] * vReal[i];
            }
            hoeveelste++;
          }
        }
        correlatie[k][j] = teller / sqrt(noemer1 * noemer2); //bereken de correlatie coëfficient
      }
    }
    digitalWrite(output, LOW);
    minimum1 = 0;
    minimum2 = 0;
    for (int k = 0; k < AMOUNT_ARRAY; k++)
    {
      for (int j = 0; j < SAMPLES_PER_OUTPUT; j++)
      {
        if (correlatie[k][j] > correlatie[minimum1][minimum2])
        {
          minimum1 = k;
          minimum2 = j;
        }
      }
    }
    if (minimum1 == vorige && correlatie[minimum1][minimum2] > MINIMUM_CORRELATION)
    {
      aantalkeren++;
    }
    else if (correlatie[minimum1][minimum2] > MINIMUM_CORRELATION)
    {
      aantalkeren = 0;
      vorige = minimum1;
    }
    if (aantalkeren == AMOUNT_SAME)
    {
      output = minimum1;
      aantalkeren = 0;
    }
    digitalWrite(output, HIGH);
  }
}

#endif

#ifdef DEMPING
int hoeveelste = 0;
double energy = 0;
for (int i = 0; i < (SAMPLES / 2); i++)
{
  double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
  if (frequency > MINFREQ && frequency < MAXFREQ)
  {
    //if (samples[hoeveelste][0][0] >= MIN_AMPLITUDE)
    //{
    energy += vReal[i];
    //}
    hoeveelste++;
  }
}

totaal = 0;
demping = energy / totalenergy;
double tussen = demping;
//Serial.println(demping);
moving_avg[moving_pos] = tussen;

moving_pos++;
if (moving_pos >= AMOUNT_MOVING_AVG)
{
  moving_pos = 0;
}

for (int i = 0; i < AMOUNT_MOVING_AVG; i++)
{
  totaal += moving_avg[moving_pos];
}

PWM_value = totaal / AMOUNT_MOVING_AVG;
if (PWM_value >= 1)
{
  PWM_value = 1;
}
int out = round(abs(log10(PWM_value) * 255)); //kan delen door log10(a) zodat bovenste log loga() word https://www.instagram.com/p/BthYeq_BsfK/?utm_source=ig_share_sheet&igshid=1fo84lib8xveu
if (out >= 255)
{
  out = 255;
}
analogWrite(LEDPIN2, out);
}
}
#endif

void fft::averageEnergy()
{
  double gemiddelddb = 0;
  int N = 0;
  for (int i = 0; i < (SAMPLES / 2); i++)
  {
    double frequency = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
    if (frequency > MINFREQ && frequency < MAXFREQ)
    {
      gemiddelddb += vReal[i];
      N++;
    }
  }
  gemiddelddb /= N;
  //Serial.println(gemiddelddb);
}
