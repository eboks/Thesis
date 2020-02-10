#include <Arduino.h>
#include "testcode.h"
#include "config.h"

int counter;
int x;

void testcode::init(){
    pinMode(ANALOG9, INPUT);
    analogReadRes(8);          // set ADC resolution to this many bits
    analogReadAveraging(1);    // average this many readings
}

void testcode::testsamplingrate()
{
  long oldT = millis();   // record start time in milliseconds
  analogRead(ANALOG9);
  counter++;
  for (int i=0;i<1000000;i++) {
        x = analogRead(ANALOG9);
      } 
  long durT = millis() - oldT;
  Serial.println((double) durT/1000);
}