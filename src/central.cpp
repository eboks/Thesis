#include "central.h"

RF24 centralradio(CESENSE, CSNSENSE);     // nRF24L01 (CE,CSN)
RF24Network centralnetwork(centralradio); // Include the radio in the network
const uint16_t central_node = 00;         // Address of this node in Octal format ( 04,031, etc)
const uint16_t nodes[2] = {01, 02};       // Addresses of the sense nodes
byte dummy = 0;
boolean even = true;
unsigned int i;

void sendStartSignal()
{
    if(even){
        //digitalWrite(6,HIGH);
        //digitalWrite(7,LOW);
        even=false;
        i=0;
    }
    else{
        //digitalWrite(7,HIGH);
        //digitalWrite(6,LOW);
        even=true;
        i=1;
    }
    while(i<TOTALCOUPLES){
        RF24NetworkHeader header(nodes[i]);                  // (Address where the data is going)
        centralnetwork.write(header, &dummy, sizeof(dummy)); // Send the data
        //Serial.print("Start ");
        //Serial.print(i);
        //Serial.println(": ");
        //Serial.println(millis());
        i+=2;
    }
}

Ticker timer(sendStartSignal, 50, 0, MILLIS);

void central::setup()
{
    pinMode(ONBOARDLEDPIN, OUTPUT);
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
    SPI.begin();
    centralradio.begin();
    centralnetwork.begin(11, central_node);                                    //(channel, node address)
    centralradio.setDataRate(RF24_2MBPS);
    timer.start();
}

void central::run()
{
    while (1){
        timer.update();                                                       //needed for timer to work
        centralnetwork.update();

        if (centralnetwork.available()){                                     // Is there any incoming data?
            //Serial.println(millis());
            RF24NetworkHeader header;
            unsigned int incomingData;
            centralnetwork.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
            Serial.print("Response ");
            Serial.print(header.from_node);
            Serial.println(": ");
            //Serial.println(millis());
            //uint16_t node = header.from_node;
            Serial.println(incomingData,1);
        }
    }
}
