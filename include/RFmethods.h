struct message{unsigned char byte1, byte2, byte3, byte4, byte5;};
typedef struct message Message;

#define PAYLOAD_BYTES       0x05
#define RX_MODE             1
#define TX_MODE             2
#define R_REGISTER          0x00  
#define W_REGISTER          0x20  
#define R_RX_PAYLOAD        0x61  
#define W_TX_PAYLOAD        0xA0  
#define FLUSH_TX            0xE1  
#define FLUSH_RX            0xE2  
#define REUSE_TX_PL         0xE3  
#define CONFIG              0x00  
#define EN_AA               0x01  
#define EN_RXADDR           0x02  
#define SETUP_AW            0x03  
#define SETUP_RETR          0x04  
#define RF_CH               0x05  
#define RF_SETUP            0x06  
#define STATUSS             0x07  
#define OBSERVE_TX          0x08  
#define CD                  0x09  
#define RX_ADDR_P0          0x0A  
#define RX_ADDR_P1          0x0B  
#define RX_ADDR_P2          0x0C  
#define RX_ADDR_P3          0x0D  
#define RX_ADDR_P4          0x0E  
#define RX_ADDR_P5          0x0F  
#define TX_ADDR             0x10  
#define RX_PW_P0            0x11  
#define RX_PW_P1            0x12  
#define RX_PW_P2            0x13  
#define RX_PW_P3            0x14  
#define RX_PW_P4            0x15  
#define RX_PW_P5            0x16  
#define FIFO_STATUS         0x17
#define NOP_STATUS          0xFF

void initMaster(void);
void initRFRegisters(void);
bool checkRFModuleConnection(void);

void sendData(Message);
Message readData(void);
void setRFMode(unsigned char mode);

unsigned char writeRegister(unsigned char memoryMapAddress, unsigned char dataByte);
unsigned char readRegister(unsigned char memoryMapAddress);
unsigned char executeSPI(unsigned char dataByte);

bool checkRX_DR(void);
bool checkTX_DS(void);
bool checkMAX_RT(void);
unsigned char checkStatus(void);
void clearRX_DR(void);
void clearTX_DS(void);
void clearMAX_RT(void);

void flush(void);