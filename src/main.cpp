#include "main.h"

sense mysense; //Create an instance of the class sense
sweep mysweep; //Create an instance of the class sweep
central mycentral; //create an instance of the class central

void setup()
{
  Serial.begin(115200); // baud rate is ignored with Teensy USB ACM i/o
  digitalWrite(LEDPIN, HIGH);
  delay(2000); // LED on for 2 second
  digitalWrite(LEDPIN, LOW);
  delay(1000); // wait in case serial monitor still opening
  #ifdef SWEEP
  mysweep.setup();
  mysweep.run();
  #endif
  #ifdef SENSE
  mysense.setup();
  mysense.run();
  #endif
  #ifdef CENTRAL
  mycentral.setup();
  mycentral.run();
  #endif
}

void loop()
{
  delay(1000);
  //do nothing
}


