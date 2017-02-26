//---------------------------------------------------
//                   spi.h
//---------------------------------------------------

#ifndef SPI_H_
#define SPI_H_


#define SPI0_TX_BUFFER_LENGTH 140
#define SPI0_RX_BUFFER_LENGTH 140

#define SPI1_TX_BUFFER_LENGTH 260
#define SPI1_RX_BUFFER_LENGTH 260

#define format_8bit   8
#define format_16bit  16

extern struct stSPI0 SPI_1;
extern struct stSPI1 SPI_2;

enum eSPI_Divider
{
  eSPI_Divider_2,
  eSPI_Divider_4,
  eSPI_Divider_8,
  eSPI_Divider_16,
  eSPI_Divider_32,
  eSPI_Divider_64,
  eSPI_Divider_128,
  eSPI_Divider_256
};

struct sStatusSPI
{
  unsigned char TxComplete  :1;
  unsigned char RxComplete  :1;
  unsigned char DMAComplete :1;
  unsigned char Busy        :1;
  unsigned char HoldCS      :1;
};

struct stSPI0
{
  unsigned char TxBuffer[SPI0_TX_BUFFER_LENGTH];
  unsigned char RxBuffer[SPI0_RX_BUFFER_LENGTH];
  unsigned char TxCounter;
  unsigned char TxCount;
  unsigned char RxCounter;
  unsigned char RxCount;
  struct sStatusSPI Status;
  void (*afterDMA)(void);
  void (*afterTx)(void);
  void (*afterRx)(void);
};

struct stSPI1
{
  unsigned char TxBuffer[SPI1_TX_BUFFER_LENGTH];
  unsigned char RxBuffer[SPI1_RX_BUFFER_LENGTH];
//  unsigned char RxBuffer2[SPI1_RX_BUFFER_LENGTH];
  unsigned char TxCounter;
  unsigned char TxCount;
  unsigned char RxCounter;
  unsigned char RxCount;
  struct sStatusSPI Status;
  void (*afterDMA)(void);
  void (*afterTx)(void);
  void (*afterRx)(void);
};



void SPI_1_Handler(void);
void SPI_1_DMA_Mode_Init(void);
void SPI_1_ChangeBitFormat(unsigned char BitFormat);
void ClearWhatever(void *Ptr, unsigned short Count);

void SPI_2_Handler(void);
void SPI_2_DMA_Mode_Init(void);
//void SPI_2_Init(void);
unsigned char SPI_2_SendFrame(unsigned char ByteCount,const void *PtrToBuffer, void (*afterTxFunc)(void), void (*afterRxFunc)(void));
//void SPI_2_ReadBuffer(unsigned char ByteCount, void *PtrToBuffer);
//unsigned char SPI_2_CheckBusy(void);



#endif




