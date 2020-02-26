#include "main.h"

testcode test; //create an instance of the class testcode

fft myfft; //Create an instance of the class fft

void spitransfer(int code);

RF24 radiomain(CE, CSN); // CE, CSN
const byte thisSlaveAddress[5] = {'R','x','A','A','A'};
char startsweepmain[10] = "test";

void setup()
{
  Serial.begin(115200); // baud rate is ignored with Teensy USB ACM i/o
  pinMode(LEDPIN, OUTPUT);
  pinMode(CONTROL, OUTPUT);
  pinMode(CS1, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(CSN, OUTPUT);
  pinMode(SAMPLE, INPUT);

  pinMode(ANALOG9, INPUT);
  analogReadRes(10);      // set ADC resolution to this many bits
  analogReadAveraging(1); // average this many readings

  digitalWrite(CS1, HIGH);
  digitalWrite(LEDPIN, HIGH);
  delay(3000); // LED on for 3 second
  digitalWrite(LEDPIN, LOW);
  delay(1000); // wait in case serial monitor still opening

#ifndef FFT_ENABLE
  digitalWrite(CONTROL, LOW); //cet CTRL pin low
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1)); //check datasheet, max freq 10Mhz, MSBFIRST mode, CPOL=0 and CPHA =1 => mode 1
  digitalWrite(CS1, LOW);
  spitransfer(0b0000011010011111); //control register

  spitransfer(0b0001111111111111); //#increments
  spitransfer(0b0010000000001000); //delta f lower bits
  spitransfer(0b0011000000000000); //delta f higher bits
  spitransfer(0b0110000000110010); //increment interval

  spitransfer(0b1000000000000000); //burst interval NOT NEEDED
  spitransfer(0b1100001000000000); //start f lower bits
  spitransfer(0b1101000000001000); //start f higher bits
  digitalWrite(CS1, HIGH);
  SPI.endTransaction();
  SPI.end();

  radiomain.begin();
  radiomain.setDataRate( RF24_2MBPS );
  radiomain.setPALevel(RF24_PA_HIGH);
  radiomain.setChannel(10);  
  radiomain.openReadingPipe(1, thisSlaveAddress); //Setting the address at which we will receive the data
  //radiomain.setPALevel(RF24_PA_MAX);     //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radiomain.startListening();            //This sets the module as receiver

#endif
}

void loop()
{
#ifdef FFT_ENABLE
  myfft.runfft(0);
#endif
#ifndef FFT_ENABLE
  if(radiomain.available()) //Looking for the data.
  {
    digitalWrite(CONTROL,HIGH);
    delay(1);
    digitalWrite(CONTROL,LOW);
    radiomain.read(&startsweepmain, sizeof(startsweepmain));
  }

#endif
}

void spitransfer(int code)
{
  digitalWrite(CS1, LOW);
  SPI.transfer16(code);
  digitalWrite(CS1, HIGH);
}