#include "sense.h"

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
unsigned int output = 0;
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
RF24Network network(radio);
const uint16_t this_node = SENSENODE; // Address of this node in Octal format ( 04,031, etc)
const uint16_t sweepnode = SWEEPNODE;
unsigned int dummydata = 0;

void sense::setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(CONTROL, OUTPUT);
  pinMode(CS1, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(CESENSE, OUTPUT);
  pinMode(CSNSENSE, OUTPUT);
  pinMode(SAMPLE, INPUT);
  pinMode(BUTTONINDICATOR, OUTPUT);
  pinMode(ANALOG3, INPUT);

  analogReadRes(10);      // set ADC resolution to this many bits
  analogReadAveraging(1); // average this many readings

  //RF SETTINGS
  SPI.begin();
  radio.begin();                //Starting the Wireless communication
  network.begin(11, this_node); //(channel, node address)
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.setRetries(3, 10); // delay, count

  //initialise arrays
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
}

void sense::run() //SWEEP DUURT 20.48 ms!!!!
{
  while (1)
  {
    network.update();
    if (network.available()) //check if the central has send something
    { //update every ... seconds
      RF24NetworkHeader header;
      unsigned int buttonState;
      network.read(header, &buttonState, sizeof(buttonState)); // Read the incoming data

      RF24NetworkHeader header1(sweepnode);                     // adress of the according sweep
      boolean succes = network.write(header1, &dummydata, sizeof(dummydata)); // Send dummy data to start the sweep
      if (succes == true)
      {
        dofft();
#ifdef DEMPING
        updateEnLED();
#else
        updateCorLED();
#endif
        if (plotinterval >= 25) //plot the values once in 25 times;
        {
          plotinterval = 0;
          fftPlot(); /*
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

void sense::dofft()
{
  for (int i = 0; i < SAMPLES; i++)
  {
    microseconds = micros(); //Overflows after around 70 minutes!
    vReal[i] = analogRead(ANALOG3)*3.3/1024;
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

void sense::fftPlot()
{
  //teller = 0;
  for (int i = 0; i < (SAMPLES / 2); i++)
  {
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
    if (i>500&&i<700) //840 samples bij 41000 || 1639  bij 80000
    {                      //only plot the usefull range
      //Serial.print(i);
      //Serial.print(": ");
      Serial.println(vReal[i]/4096*1000, 1); //View only this line in serial plotter to visualize the bins, 1 is 1 number after the colon
      //teller++;
    }
  }
  //Serial.println(teller);
}

void sense::updateEnLED()
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
  }
  else
  {

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
    unsigned int out = round(abs(log10(PWM_value) * 255)); //kan delen door log10(a) zodat bovenste log loga() word https://www.instagram.com/p/BthYeq_BsfK/?utm_source=ig_share_sheet&igshid=1fo84lib8xveu
    if (out >= 255)
    {
      out = 255;
    }
    RF24NetworkHeader header(00);                     // adress of the according sweep
    network.write(header, &out, sizeof(out)); // Send dummy data to start the sweep
    //analogWrite(LEDPIN, out);
  }
}

void sense::updateCorLED()
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
  }
  else
  {
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
    //digitalWrite(output, LOW);
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
    //digitalWrite(output, HIGH);
    RF24NetworkHeader header(00);                     // adress of the central
    network.write(header, &output, sizeof(output)); // Send the output to the central
  }
}

void sense::averageEnergy()
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
