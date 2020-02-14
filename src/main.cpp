#include "main.h"

testcode test; //create an instance of the class testcode

fft myfft; //Create an instance of the class fft

void spitransfer(int code);

int teller = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);       // baud rate is ignored with Teensy USB ACM i/o
  pinMode(LEDPIN, OUTPUT);  
  pinMode(BUTTONPIN, INPUT);
  pinMode(CONTROL, OUTPUT);   
  pinMode(CS, OUTPUT);    
  pinMode(ENDSWEEP, INPUT);   
 

  pinMode(ANALOG9, INPUT);
  analogReadRes(10);          // set ADC resolution to this many bits
  analogReadAveraging(1);    // average this many readings
  
  digitalWrite(CS,HIGH);
  digitalWrite(LEDPIN,HIGH);   
  delay(3000);   // LED on for 3 second
  digitalWrite(LEDPIN,LOW);    
  delay(1000);   // wait in case serial monitor still opening

  #if PROGRAM_ENABLE
  /*SPI program here*/  //MOSI to SDATA, SS to FSYNC,SCLK to SCLK to program!!
  //SPISettings mySettting(10000000, MSBFIRST, SPI_MODE1) //check datasheet, max freq 10Mhz, MSBFIRST mode, CPOL=0 and CPHA =1 => mode 1
  digitalWrite(CONTROL,LOW);   //cet CTRL pin low
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1)); //check datasheet, max freq 10Mhz, MSBFIRST mode, CPOL=0 and CPHA =1 => mode 1
  digitalWrite(CS,LOW);
  spitransfer(0b0000011010011111);  //control register
  spitransfer(0b0001001111101000);  //#increments
  spitransfer(0b0010000000100100);  //delta f lower bits
  spitransfer(0b0011000000000000);  //delta f higher bits
  spitransfer(0b0110000001100100);  //increment interval
  spitransfer(0b1000000000000000);  //burst interval NOT NEEDED
  spitransfer(0b1100001000000000);  //start f lower bits
  spitransfer(0b1101000000001000);  //start f higher bits
  digitalWrite(CS,HIGH);
  SPI.endTransaction();
  SPI.end();
  #endif
  digitalWrite(CONTROL,HIGH);   //end sweep
  delay(1);
  digitalWrite(CONTROL,LOW);   //end sweep
  }

void loop() {
  // put your main code here, to run repeatedly:
  #if FFT_ENABLE
  myfft.runfft(0);
  #endif
 /* if(digitalRead(ENDSWEEP)==true){
  digitalWrite(CONTROL,HIGH);   //end sweep
  delay(1);
  digitalWrite(CONTROL,LOW);   //end sweep
  delay(100);
  digitalWrite(CONTROL,HIGH);   //start sweep
  delay(1);
  digitalWrite(CONTROL,LOW);   //start sweep
  }*/
  
  //delay(1000);  //Repeat the process every second OR:
  //while(1);       //Run code once
}

void spitransfer(int code){
  //digitalWrite(CS,LOW);
  SPI.transfer16(code);
 // digitalWrite(CS,HIGH);
  //delay(1);
}
