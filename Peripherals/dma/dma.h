//---------------------------------------------------
//                   dma.h
//---------------------------------------------------
/*
#include "eltelnet\eltelnet.h"

#ifndef DMA_H_
  #define DMA_H_

#define SPI2_DEVICE_LCD       1
#define SPI2_DEVICE_FLASH     2


#define DMA_BUF_1   0
#define DMA_BUF_2   1
#define DMA_BUF_3   2


struct sDMA_Status
{
  unsigned char Tx1Complete :1;
  unsigned char Rx1Complete :1;
  unsigned char Tx2Complete :1;
  unsigned char Rx2Complete :1;
  unsigned char Tx3Complete :1;
  unsigned char Rx3Complete :1;
};

struct sDMA_Transmision
{
  struct eltelnet_frame RxFrame;
  struct eltelnet_frame TxFrame;
  struct sDMA_Status Status;
};



extern struct sDMA_Transmision DMA_Transmission;
extern struct stSPI0 sSPI1;
extern struct stSPI1 sSPI2;



void SPI_1_DMA_SendFrame( void *TxBuffer, void *RxBuffer, unsigned short Count);
void SPI_2_DMA_SendFrame( void *TxBuffer, void *RxBuffer, unsigned short Count, unsigned char Device);

void UART4_DMA_ReceiveFrame(void *RxBuffer, unsigned char Count);
void UART4_DMA_SendFrame(void *TxBuffer, unsigned short Count);

void USART1_DMA_ReceiveFrame(void *RxBuffer, unsigned char Count);
void USART1_DMA_SendFrame(void *TxBuffer, unsigned short Count);

#endif


*/
