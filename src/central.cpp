#include "central.h"



RF24 centralradio(10, 9);                 // nRF24L01 (CE,CSN)
RF24Network centralnetwork(centralradio); // Include the radio in the network
const uint16_t central_node = 00;         // Address of this node in Octal format ( 04,031, etc)
const uint16_t nodes[2] = {01, 02};       // Addresses of the sense nodes
unsigned int dummy = 0;
unsigned int i = 0;

void sendStartSignal()
{
    if ((i % 2) == 0) //alternate  starting position of i from odd to even
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    for (i; i <= sizeof(nodes); i += 2)                       //let all the even/odd receivers use the spiral
    {
        RF24NetworkHeader header(nodes[i]);                  // (Address where the data is going)
        centralnetwork.write(header, &dummy, sizeof(dummy)); // Send the data
    }
}

Ticker timer(sendStartSignal, 50, 0, MILLIS);


void central::setup()
{
    pinMode(ONBOARDLEDPIN, OUTPUT);
    SPI.begin();
    centralradio.begin();
    centralnetwork.begin(11, central_node); //(channel, node address)
    centralradio.setDataRate(RF24_2MBPS);
    timer.start();
}

void central::run()
{
    while (1)
    {
        timer.update(); //needed for timer to work
        centralnetwork.update();
        if (centralnetwork.available())
        { // Is there any incoming data?
            RF24NetworkHeader header;
            unsigned int incomingData;
            centralnetwork.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
            uint16_t node = header.from_node;
            //TODO: output toevoegen voor elke node
        }
    }
}
