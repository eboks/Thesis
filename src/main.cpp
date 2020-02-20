#include "main.h"

testcode test; //create an instance of the class testcode

fft myfft; //Create an instance of the class fft

void spitransfer(int code);

int teller = 0;
unsigned long timing;
unsigned int CLK_period_us= 1000;
int increment = 0;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

void setup() {
  Serial.begin(115200);       // baud rate is ignored with Teensy USB ACM i/o
  pinMode(LEDPIN, OUTPUT);  
  pinMode(BUTTONPIN, INPUT);
  pinMode(CONTROL, OUTPUT);   
  pinMode(CS1, OUTPUT);    
  pinMode(ENDSWEEP, OUTPUT);   
  pinMode(VERZENDCONTROL, OUTPUT);   
  pinMode(ONTVANGCONTROL, INPUT);  
  pinMode(CE_, OUTPUT);
  pinMode(CSN, OUTPUT); 
  pinMode(9,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(8,OUTPUT);

  pinMode(ANALOG9, INPUT);
  analogReadRes(10);          // set ADC resolution to this many bits
  analogReadAveraging(1);    // average this many readings
  
  digitalWrite(CS1,HIGH);
  digitalWrite(LEDPIN,HIGH);   
  delay(3000);   // LED on for 3 second
  digitalWrite(LEDPIN,LOW);    
  delay(1000);   // wait in case serial monitor still opening

  #ifndef FFT_ENABLE
  /*SPI program here*/  //MOSI to SDATA, SS to FSYNC,SCLK to SCLK to program!!
  //SPISettings mySettting(10000000, MSBFIRST, SPI_MODE1) //check datasheet, max freq 10Mhz, MSBFIRST mode, CPOL=0 and CPHA =1 => mode 1
  digitalWrite(CONTROL,LOW);   //cet CTRL pin low
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1)); //check datasheet, max freq 10Mhz, MSBFIRST mode, CPOL=0 and CPHA =1 => mode 1
  digitalWrite(CS1,LOW);
  spitransfer(0b0000011010011111);  //control register

  /*spitransfer(0b0001001111101000);  //#increments
  spitransfer(0b0010000000100100);  //delta f lower bits
  spitransfer(0b0011000000000000);  //delta f higher bits
  spitransfer(0b0110000001100100);  //increment interval*/

  spitransfer(0b0001111111111111);  //#increments
  spitransfer(0b0010000000001000);  //delta f lower bits
  spitransfer(0b0011000000000000);  //delta f higher bits
  spitransfer(0b0110000000110010);  //increment interval

  spitransfer(0b1000000000000000);  //burst interval NOT NEEDED
  spitransfer(0b1100001000000000);  //start f lower bits
  spitransfer(0b1101000000001000);  //start f higher bits
  digitalWrite(CS1,HIGH);
  SPI.endTransaction();
  SPI.end();
  #endif

  /*initMaster();
  while(checkRFModuleConnection()==false){
    digitalWrite(9,HIGH);   
    delay(1000);   // LED on for 1 second
    digitalWrite(9,LOW); 
    delay(1000);
    }//there is a problem with the SPI
  initRFRegisters();*/
  }

void loop() {

  #ifdef FFT_ENABLE
  myfft.runfft(0);
  #endif
/*
  #ifndef FFT_ENABLE
    
    setRFMode(RX_MODE);
    Message response;
    response.byte1=2;  
    response.byte2=4;      
    response.byte3=6;      
    response.byte4=8;
    response.byte5=10;
    sendData(response);
    
    while(1){
        if((checkStatus()>>6)==1){          //there is new data in the RX FIFO
            clearRX_DR();
            Message message=readData();
            flush();
            if(message.byte1==1){
              digitalWrite(VERZENDCONTROL, HIGH);
              delay(1);
              digitalWrite(VERZENDCONTROL, LOW);
              delay(1);
            }
            sendData(response);
        }
        else if((checkStatus()>>5)==1){     //succesfully sended the data
            clearTX_DS();
        }
        else if((checkStatus()>>4)==1){     //maximum number of retries is reached
            clearMAX_RT();
            //sendData(response);             //try again
        }
    }
  #endif 
*/
}

void spitransfer(int code){
  digitalWrite(CS1,LOW);
  SPI.transfer16(code);
  digitalWrite(CS1,HIGH);
}