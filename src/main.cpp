#include <Arduino.h>
#include <arduinoFFT.h>

#include "config.h"
#include "main.h"
#include "testcode.h"

testcode test;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);       // baud rate is ignored with Teensy USB ACM i/o
  pinMode(LEDPIN, OUTPUT);  
  
  digitalWrite(LEDPIN,HIGH);   
  delay(1000);   // LED on for 1 second
  digitalWrite(LEDPIN,LOW);    
  delay(3000);   // wait in case serial monitor still opening

  test.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  test.testsamplingrate();

}