#include "RFmethods.h"
#include <Arduino.h> 
#include <SPI.h>
#include "config.h"

void initRFRegisters(void){
    writeRegister(CONFIG, 0b00001010);          //power up mode
    writeRegister(EN_AA, 0b00000001);           //auto ack. data pipe 0 enabled
    writeRegister(EN_RXADDR, 0b000000001);      //enable data pipe 0
    writeRegister(SETUP_AW, 0b00000011);        //address width = 5 bytes
    writeRegister(SETUP_RETR, 0b00000000);      //auto re-transmit delay - wait 250+86uS, disabled
    writeRegister(RF_CH, 0b01001000);           //sets the frequency channel to 1100
    writeRegister(RF_SETUP, 0b00001111);        //data rate 2Mbps, output power TX mode 0dbm, setup LNA gain
    writeRegister(STATUSS, 0b01110000);         //clear status
    //writeRegister(RX_ADDR_P0,0xE7E7E7E7E7);   //0xE7E7E7E7E7 is default for datapipe0, so not needed to initiallize
    //writeRegister(TX_ADDR_P0,0xE7E7E7E7E7);   //0xE7E7E7E7E7 is default for datapipe0, so not needed to initiallize
    writeRegister(RX_PW_P0,PAYLOAD_BYTES);      //number of bytes in RX payload = 5;
    flush();
    setRFMode(RX_MODE);
    return;
}

bool checkRFModuleConnection(void){
    writeRegister(RF_CH, 0b01001000);
    if(readRegister(CONFIG==72)){
        return true;
    }
    else{
        return false;
    }
}

void setRFMode(unsigned char mode){
    switch(mode){
        case RX_MODE:
            writeRegister(CONFIG, 0b00001111);  //CRC enabled, 2bytes, power up, RX
            digitalWrite(CE_, HIGH);
            break;
        case TX_MODE:
            writeRegister(CONFIG, 0b00001110);  //CRC enabled, 2 bytes, power up, TX
            digitalWrite(CE_, LOW);
            break;
    }
    return;
}

void sendData(Message message){
    setRFMode(TX_MODE);
    digitalWrite(CSN, LOW);
    executeSPI(W_TX_PAYLOAD);
    executeSPI(message.byte1);
    executeSPI(message.byte2);
    executeSPI(message.byte3);
    executeSPI(message.byte4);
    executeSPI(message.byte5);
    digitalWrite(CSN, HIGH);
    digitalWrite(CE_, HIGH);
    delay(1);
    digitalWrite(CE_, LOW);
    setRFMode(RX_MODE);
    return;
}

Message readData(void){
    Message message;
    digitalWrite(CSN, LOW);
    executeSPI(R_RX_PAYLOAD);
    message.byte1=executeSPI(0x00);
    message.byte2=executeSPI(0x00);
    message.byte3=executeSPI(0x00);
    message.byte4=executeSPI(0x00);
    message.byte5=executeSPI(0x00);
    digitalWrite(CSN, HIGH);
    return message;
}

void initMaster(void){
    SPI.begin();
    SPI.beginTransaction(SPISettings(100000, LSBFIRST, SPI_MODE1));
    return;
}

unsigned char executeSPI(unsigned char dataByte){    
    unsigned char response;
    response=SPI.transfer(dataByte);
    delay(1);
    return response;
}

unsigned char writeRegister(unsigned char memoryMapAddress, unsigned char dataByte){
    unsigned char status;
    digitalWrite(CE_, LOW);                                         //standby-I
    delay(1);
    digitalWrite(CSN, LOW);                                            //Before transmission CSN has to be cleared
    delay(1);                              
    status=executeSPI(W_REGISTER|memoryMapAddress); //Instruction word: MSBit to LSBit (one byte)
    executeSPI(dataByte);                          //Data bytes: LSByte to MSByte, MSBit in each byte first
    delay(1);
    digitalWrite(CSN, HIGH);                                          //After transmission CSN has to be set
    delay(1);
    digitalWrite(CE_, HIGH);                                          //out of standby-I
    return status;
}

unsigned char readRegister(unsigned char memoryMapAddress){
    unsigned char status;
    unsigned char registerValue;
    digitalWrite(CSN, LOW);
    delay(1);
    status=executeSPI(R_REGISTER|memoryMapAddress);
    registerValue=executeSPI(0x00);
    delay(1);
    digitalWrite(CSN, HIGH);
    return registerValue;
}

unsigned char checkStatus(void){
    unsigned char status;
    digitalWrite(CSN, LOW);                                         
    delay(1);
    status=executeSPI(NOP_STATUS);
    delay(1);
    digitalWrite(CSN, HIGH);
    return status;
}

void clearRX_DR(void){
    writeRegister(STATUSS, 0b01000000);
}

void clearTX_DS(void){
    writeRegister(STATUSS, 0b00100000);
}

void clearMAX_RT(void){
    writeRegister(STATUSS, 0b00010000);
}

void flush(void){
    digitalWrite(CSN, LOW); 
    delay(1);
    executeSPI(FLUSH_TX);
    delay(1);
    digitalWrite(CSN, HIGH);
    delay(1);
    digitalWrite(CSN, LOW); 
    delay(1);
    executeSPI(FLUSH_RX);
    delay(1);
    digitalWrite(CSN, HIGH);
    return;
}