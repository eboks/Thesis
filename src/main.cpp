#include "main.h"

testcode test; //create an instance of the class testcode

fft myfft; //Create an instance of the class fft

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);       // baud rate is ignored with Teensy USB ACM i/o
  pinMode(LEDPIN, OUTPUT);  
  pinMode(BUTTONPIN, INPUT);  

  pinMode(ANALOG9, INPUT);
  analogReadRes(10);          // set ADC resolution to this many bits
  analogReadAveraging(1);    // average this many readings
  
  digitalWrite(LEDPIN,HIGH);   
  delay(3000);   // LED on for 3 second
  digitalWrite(LEDPIN,LOW);    
  delay(1000);   // wait in case serial monitor still opening

}

void loop() {
  // put your main code here, to run repeatedly:
  myfft.runfft(0);

  //delay(1000);  //Repeat the process every second OR:
  //while(1);       //Run code once
}
