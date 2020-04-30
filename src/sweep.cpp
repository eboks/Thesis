#include "sweep.h"

RF24 sweepradio(CESWEEP, CSNSWEEP); // CE, CSN
RF24Network sweepnetwork(sweepradio);    // Include the radio in the network
const uint16_t this_node = SWEEPNODE; // Address of our node in Octal format ( 04,031, etc)
unsigned long currenttime = 0;

void blink(){
    digitalWrite(INTERRUPT, HIGH);
    delay(1);
    digitalWrite(INTERRUPT, LOW);
}

void sweep::setup()
{
    pinMode(CONTROL, OUTPUT);
    pinMode(CS1, OUTPUT);
    pinMode(TRISTATE, OUTPUT);
    pinMode(CESWEEP, OUTPUT);
    pinMode(CSNSWEEP, OUTPUT);
    pinMode(SYNCOUT, INPUT);
    pinMode(INTERRUPT, OUTPUT);

    digitalWrite(TRISTATE, HIGH);                                       //has to be high for the oscillator to be on
    digitalWrite(CS1, HIGH);
    digitalWrite(CONTROL, LOW);                                         //cet CTRL pin low

    SPI.begin();
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1));   //check datasheet, max freq 10Mhz, MSBFIRST mode, CPOL=0 and CPHA =1 => mode 1
    digitalWrite(CS1, LOW);
    spitransfer(0b0000011010011111); //control register
    spitransfer(0b0001101111111111); //#increments
    spitransfer(0b0010000000000001); //delta f lower bits
    spitransfer(0b0011000000000000); //delta f higher bits
    spitransfer(0b0110000101011010); //increment interval
    spitransfer(0b1000000000000000); //burst interval NOT NEEDED
    spitransfer(0b1100000100000000); //start f lower bits  
    spitransfer(0b1101000000000010); //start f higher bits
    digitalWrite(CS1, HIGH);
    SPI.endTransaction();
    SPI.end();
    SPI.begin();                        //spi for the communication
    sweepradio.begin();
    sweepnetwork.begin(11, this_node);  //(channel, node address)
    sweepradio.setDataRate(RF24_2MBPS);
    sweepradio.setPALevel(RF24_PA_MAX);

    attachInterrupt(digitalPinToInterrupt(SYNCOUT), blink, RISING);
}



void sweep::run()
{
    while (1){
        sweepnetwork.update();
        if (sweepnetwork.available()){                                       //Looking for the data.
            RF24NetworkHeader header;
            unsigned int incomingData;
            digitalWrite(CONTROL, HIGH);                                    //start the sweep
            delay(1);
            digitalWrite(CONTROL, LOW);
            sweepnetwork.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
            currenttime = millis();
        }
        else if (currenttime + 500 < millis()){
            SPI.begin();
            sweepradio.begin();
            sweepnetwork.begin(11, this_node);                              //(channel, node address)
            sweepradio.setDataRate(RF24_2MBPS);
            sweepradio.setPALevel(RF24_PA_MAX);
            currenttime = millis();
        }
    }
}

void sweep::spitransfer(int code){
    digitalWrite(CS1, LOW);
    SPI.transfer16(code);
    digitalWrite(CS1, HIGH);
}