#include <Arduino.h>

#include "config.h"
#include "main.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPIN, OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LEDPIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); 
}