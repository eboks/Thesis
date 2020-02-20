#include "main.h"

testcode test; //create an instance of the class testcode

fft myfft; //Create an instance of the class fft

void spitransfer(int code);

int teller = 0;
unsigned long timing;
unsigned int CLK_period_us= 1000;
int increment = 0;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

void setup() {
  Serial.begin(115200);       // baud rate is ignored with Teensy USB ACM i/o
  pinMode(LEDPIN, OUTPUT);  
  pinMode(CONTROL, OUTPUT);   
  pinMode(CS1, OUTPUT);     
  pinMode(VERZENDCONTROL, OUTPUT);   

  pinMode(ANALOG9, INPUT);
  analogReadRes(10);          // set ADC resolution to this many bits
  analogReadAveraging(1);    // average this many readings
  
  digitalWrite(CS1,HIGH);
  digitalWrite(LEDPIN,HIGH);   
  delay(3000);   // LED on for 3 second
  digitalWrite(LEDPIN,LOW);    
  delay(1000);   // wait in case serial monitor still opening

  #ifndef FFT_ENABLE
  digitalWrite(CONTROL,LOW);        //cet CTRL pin low
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1)); //check datasheet, max freq 10Mhz, MSBFIRST mode, CPOL=0 and CPHA =1 => mode 1
  digitalWrite(CS1,LOW);
  spitransfer(0b0000011010011111);  //control register

  spitransfer(0b0001111111111111);  //#increments
  spitransfer(0b0010000000001000);  //delta f lower bits
  spitransfer(0b0011000000000000);  //delta f higher bits
  spitransfer(0b0110000000110010);  //increment interval

  spitransfer(0b1000000000000000);  //burst interval NOT NEEDED
  spitransfer(0b1100001000000000);  //start f lower bits
  spitransfer(0b1101000000001000);  //start f higher bits
  digitalWrite(CS1,HIGH);
  SPI.endTransaction();
  SPI.end();
  #endif
  }

void loop() {
  #ifdef FFT_ENABLE
  myfft.runfft(0);
  #endif
}

void spitransfer(int code){
  digitalWrite(CS1,LOW);
  SPI.transfer16(code);
  digitalWrite(CS1,HIGH);
}