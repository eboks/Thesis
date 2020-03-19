#include "sweep.h"

RF24 radiomain(CESWEEP, CSNSWEEP); // CE, CSN
const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
char startsweepmain[10] = "test";
unsigned long currenttime = 0;

void blink()
{
    digitalWrite(INTERRUPT, HIGH);
    delay(1);
    digitalWrite(INTERRUPT, LOW);
}

void sweep::setup()
{
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
    pinMode(TRISTATE, OUTPUT);
    pinMode(CESWEEP, OUTPUT);
    pinMode(CSNSWEEP, OUTPUT);
    pinMode(CESENSE, OUTPUT);
    pinMode(CSNSENSE, OUTPUT);
    pinMode(SYNCOUT, OUTPUT);
    pinMode(BUTTONINDICATOR, OUTPUT);
    pinMode(SYNCOUT, INPUT);
    pinMode(INTERRUPT, OUTPUT);

    digitalWrite(TRISTATE, HIGH); //has to be high for the oscillator to be on

    digitalWrite(TRISTATE, HIGH); //has to be high for the oscillator to be on
    digitalWrite(CS1, HIGH);
    digitalWrite(CONTROL, LOW); //cet CTRL pin low
    SPI.begin();
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1)); //check datasheet, max freq 10Mhz, MSBFIRST mode, CPOL=0 and CPHA =1 => mode 1
    digitalWrite(CS1, LOW);
    spitransfer(0b0000011010011111); //control register

    spitransfer(0b0001101111111111); //#increments
    /*spitransfer(0b0010000000000011); //delta f lower bits
  spitransfer(0b0011000000000000); //delta f higher bits*/
    spitransfer(0b0010000000000011); //delta f lower bits
    spitransfer(0b0011000000000000); //delta f higher bits
    spitransfer(0b0110000101011010); //increment interval

    spitransfer(0b1000000000000000); //burst interval NOT NEEDED

    spitransfer(0b1100011100000000); //start f lower bits  100 1110 0010 0000 //25-50kHz
    spitransfer(0b1101000000000001);
    /*spitransfer(0b1100111100000000); //start f lower bits  100 1110 0010 0000 //25-50kHz
  spitransfer(0b1101000000000001); //start f higher bits*/

    digitalWrite(CS1, HIGH);
    SPI.endTransaction();
    SPI.end();

    radiomain.begin();
    radiomain.setDataRate(RF24_2MBPS);
    radiomain.setPALevel(RF24_PA_MAX);
    radiomain.setChannel(10);
    radiomain.openReadingPipe(1, thisSlaveAddress); //Setting the address at which we will receive the data
    //radiomain.setPALevel(RF24_PA_MAX);     //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
    radiomain.startListening(); //This sets the module as receiver

    attachInterrupt(digitalPinToInterrupt(SYNCOUT), blink, RISING);
}

void sweep::run()
{
    while (1)
    {
        if (radiomain.available()) //Looking for the data.
        {
            digitalWrite(CONTROL, HIGH);
            delay(1);
            digitalWrite(CONTROL, LOW);
            radiomain.read(&startsweepmain, sizeof(startsweepmain));
            currenttime = millis();
        }
        else if (currenttime + 500 < millis())
        {
            radiomain.begin();
            radiomain.setDataRate(RF24_2MBPS);
            radiomain.setPALevel(RF24_PA_MAX);
            radiomain.setChannel(10);
            radiomain.openReadingPipe(1, thisSlaveAddress); //Setting the address at which we will receive the data
            radiomain.startListening();                     //This sets the module as receiver
            currenttime = millis();
        }
    }
}

void sweep::spitransfer(int code)
{
    digitalWrite(CS1, LOW);
    SPI.transfer16(code);
    digitalWrite(CS1, HIGH);
}