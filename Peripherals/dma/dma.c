//---------------------------------------------------
//                   dma.c
//---------------------------------------------------
#include <ctl_api.h>
#include "stm32l1xx.h"

#include "peripherals\timer\timer.h"
#include "peripherals\spi\spi.h"
#include "peripherals\adc\adc.h"
#include "dma.h"

#include "boards\board.h"
#include "main.h"
#include "eltelnet\eltelnet.h"
#include "setup\setup.h"

struct sDMA_Transmision DMA_Transmission;
unsigned char Errors1;

extern unsigned char bufferRead;
extern unsigned char bufferWrite;

struct stSPI0 sSPI1;
struct stSPI1 sSPI2;

void bufferInc(void)
{
  switch(bufferWrite)
  {
    case 0: 
      bufferRead = 2; 
      bufferWrite = 1; 
      break;
    case 1: 
      bufferRead = 0; 
      bufferWrite = 2; 
      break;
    case 2: 
      bufferRead = 1; 
      bufferWrite = 0;
      break;
  }
}

//=============================================================================
//                            OBSLUGA PRZERWAN
//=============================================================================


/// ADC1
void DMA1_Channel1_IRQHandler(void) 
{  
  NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);

  /* Test if transfer completed on DMA channel 1 */
  if ((DMA1->ISR & DMA_ISR_TCIF1) != 0) 
  {
    /* Disable DMA Channel 1 to write in CNDTR*/
    CLEAR_BIT(DMA1_Channel1->CCR, DMA_CCR1_EN);  
    
    /* Reload the number of DMA transfer to be performs on DMA channel 1 */
    DMA1_Channel1->CNDTR = MAX_R_SAMPLE; 
    
//    /* Enable again DMA Channel 1 */
//    SET_BIT(DMA1_Channel1->CCR, DMA_CCR1_EN); 
    
    DMA1->IFCR |= DMA_IFCR_CTCIF1; /* Clear the flag */

    /// Toggle blue led on PB9
//    GPIOB->ODR ^= (1<<9);
  }
  else if ((DMA1->ISR & DMA_ISR_TEIF1) != 0) /* Test if transfer error on DMA channel 1 */
  {
    /* Report an error */
    DMA1->IFCR |= DMA_IFCR_CTEIF1; /* Clear the flag */
  }
  else
  {
    ;/* report unexpected DMA interrupt occurrence */
  }
  
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}



/// SPI1 RX (CMX)
void DMA1_Channel2_IRQHandler(void) 
{
  sSPI1.Status.DMAComplete = 1;
  
  if(sSPI1.Status.HoldCS == 0) 
    CODEC_CS_1;

  DMA1->IFCR = DMA_IFCR_CTCIF2;
  NVIC_ClearPendingIRQ(DMA1_Channel2_IRQn);
}

/// SPI1 TX (CMX)
void DMA1_Channel3_IRQHandler(void) 
{
  DMA1->IFCR = DMA_IFCR_CTCIF3;
  NVIC_ClearPendingIRQ(DMA1_Channel3_IRQn);
}

/**
 *  ######  ####      ##
 *    #     #   #    #  #
 *    #     ####     #  #
 *    #     #  #     #  #
 *    #     #   #     ##
 */
/// UART4_RX (ELTELNET)
void DMA2_Channel3_IRQHandler(void)
{
  NVIC_ClearPendingIRQ(DMA2_Channel3_IRQn);

  if(DMA2->ISR & DMA_ISR_TEIF3)
  {
    DMA2->IFCR = DMA_IFCR_CTEIF3;
    CLEAR_BIT(DMA2_Channel3->CCR, DMA_CCR3_EN);   
  }
  if(DMA2->ISR = DMA_ISR_TCIF3)
  {
    DMA2->IFCR = DMA_IFCR_CTCIF3;
    
    UART4_DMA_ReceiveFrame(&DMA_Transmission.RxFrame, sizeof(struct eltelnet_frame));
    if(CheckFrame(&DMA_Transmission.RxFrame) == F_RESULT_OK)
    {
      TimerStart(eTimer_TimeoutTr0, 500); // 0,5s
      TR1.Status = TR_STATE_OK;
      TR1.WasTr = 1;
      TimerTr0Send();
      DE2_1; 

      if(DMA_Transmission.RxFrame.DstAddress.Entire != 0)
      {
        AddRxFrameToFIFO(&DMA_Transmission.RxFrame, &TR1);
      }
      else
      {
        /// Tu jest ramka z szumem...
        if( *((unsigned short*)&DMA_Transmission.RxFrame.Data[0]) != 0 )
        {
          TR1.NextAddr.AddrTR.Entire = *((unsigned short*)&DMA_Transmission.RxFrame.Data[0]);
          if(TR1.NextAddr.AddrTR.Net != (Sep.Setup.Address.Net - 16))
          {
            Sep.Setup.Address.Net = TR1.NextAddr.AddrTR.Net + 16;
            Prepare_NoiseFrame_Address(Sep.Setup.Address);
          }
        }
      }
    }
    else
    {
      Timer_TR1_Timeout_ON();
      Errors1++;
      DE2_0;
    }

    CLEAR_BIT(DMA2_Channel3->CCR, DMA_CCR3_EN);   /// Uwaga ! trzeba kasować w STML1xx w odróżnieniu od STMF4 
  }
}

/// UART4_TX (ELTELNET)
void DMA2_Channel5_IRQHandler(void)
{
  CLEAR_BIT(DMA2_Channel5->CCR, DMA_CCR5_EN); // wlacz DMA TX

  DMA2->IFCR = DMA_IFCR_CTCIF5;
  DMA2->IFCR = DMA_IFCR_CTEIF5;
  NVIC_ClearPendingIRQ(DMA2_Channel5_IRQn);
}

/**
 *  ######  ####      ##
 *    #     #   #    #  #
 *    #     ####       # 
 *    #     #  #      #  
 *    #     #   #    ####
 */

/** 
 *  TRANSMISJA ELTELNET TR2 USART1_RX
 */
void DMA1_Channel5_IRQHandler(void)
{
  NVIC_ClearPendingIRQ(DMA1_Channel5_IRQn);

  if(DMA1->ISR & DMA_ISR_TEIF5)
  {
    DMA1->IFCR = DMA_IFCR_CTEIF5;
    CLEAR_BIT(DMA1_Channel5->CCR, DMA_CCR5_EN);   
  }
  if(DMA1->ISR = DMA_ISR_TCIF5)
  {
    DMA1->IFCR = DMA_IFCR_CTCIF5;
    
    USART1_DMA_ReceiveFrame(&DMA_Transmission.RxFrame, sizeof(struct eltelnet_frame));
    if(CheckFrame(&DMA_Transmission.RxFrame) == F_RESULT_OK)
    {
      TimerStart(eTimer_TimeoutTr2, 500); // 0,5s
      TR2.Status = TR_STATE_OK;
      TR2.WasTr = 1;
      TimerTr2Send();
      DE1_1; 

      if(DMA_Transmission.RxFrame.DstAddress.Entire != 0)
      {
        AddRxFrameToFIFO(&DMA_Transmission.RxFrame, &TR2);
      }
      else
      {
        /// Tu jest ramka z szumem...
        if( *((unsigned short*)&DMA_Transmission.RxFrame.Data[0]) != 0 )
        {
          TR2.NextAddr.AddrTR.Entire = *((unsigned short*)&DMA_Transmission.RxFrame.Data[0]);
          if(TR2.NextAddr.AddrTR.Net != (Sep.Setup.Address.Net - 16))
          {
            Sep.Setup.Address.Net = TR2.NextAddr.AddrTR.Net + 16;
            Prepare_NoiseFrame_Address(Sep.Setup.Address);
          }
        }
      }
    }
    else
    {
      Timer_TR2_Timeout_ON();
      Errors1++;
      DE1_0;
    }

    CLEAR_BIT(DMA1_Channel5->CCR, DMA_CCR5_EN);   /// Uwaga ! trzeba kasować w STML1xx w odróżnieniu od STMF4 
  }
}

/** 
 *  TRANSMISJA ELTELNET TR2 USART1_TX
 */
void DMA1_Channel4_IRQHandler(void)
{
  CLEAR_BIT(DMA1_Channel4->CCR, DMA_CCR4_EN); // wlacz DMA TX

  DMA1->IFCR = DMA_IFCR_CTCIF4;
  DMA1->IFCR = DMA_IFCR_CTEIF4;
  NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
}

//=============================================================================
//                                FUNKCJE
//=============================================================================

/**
 *  SPI_1_DMA_SendFrame obsługa CMX 
 */
void SPI_1_DMA_SendFrame( void *TxBuffer, void *RxBuffer, unsigned short Count)
{
  WRITE_REG(DMA1->IFCR, DMA_IFCR_CTCIF2);     /// wyczysc flage RX
  WRITE_REG(DMA1->IFCR, DMA_IFCR_CTCIF3);     /// wyczysc flage TX

  CLEAR_BIT(DMA1_Channel2->CCR, DMA_CCR2_EN); /// Uwaga!!! trzeba kasować w STML1xx w odróżnieniu od STMF4 <jesli chcemy zmienic zawartość rejestrow DMA>
  CLEAR_BIT(DMA1_Channel3->CCR, DMA_CCR3_EN); /// Uwaga!!! trzeba kasować w STML1xx w odróżnieniu od STMF4 

  CODEC_CS_0;

  if(RxBuffer == NULL) 
    MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_MINC, 0);
  else 
    MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_MINC, DMA_CCR2_MINC);

  DMA1_Channel2->CNDTR = Count;
  DMA1_Channel2->CMAR = (unsigned int)RxBuffer;
  
  DMA1_Channel3->CNDTR = Count;
  DMA1_Channel3->CMAR = (unsigned int)TxBuffer;
  
  sSPI1.Status.DMAComplete = 0;
  
  SET_BIT(DMA1_Channel2->CCR, DMA_CCR2_EN); // wlacz DMA RX
  SET_BIT(DMA1_Channel3->CCR, DMA_CCR3_EN); // wlacz DMA TX
}

/**
 *  UART4_DMA_xxxFrame obsługa ELTELNET jako SLAVE 
 */
void UART4_DMA_ReceiveFrame(void *RxBuffer, unsigned char Count)
{
  NVIC_DisableIRQ(DMA2_Channel3_IRQn);
  CLEAR_BIT(DMA2_Channel3->CCR, DMA_CCR3_EN);

  WRITE_REG(DMA2->IFCR, DMA_IFCR_CTCIF3 | DMA_IFCR_CTEIF3);
  NVIC_ClearPendingIRQ(DMA2_Channel3_IRQn);

  DMA2_Channel3->CNDTR = Count;
  DMA2_Channel3->CMAR  = (unsigned int)RxBuffer;
  
  NVIC_EnableIRQ(DMA2_Channel3_IRQn);
  SET_BIT(DMA2_Channel3->CCR, DMA_CCR3_EN);
}

void UART4_DMA_SendFrame(void *TxBuffer, unsigned short Count)
{
  unsigned short status;

  UART4_TRANSMIT;
  CLEAR_BIT(DMA2_Channel5->CCR, DMA_CCR5_EN);   /// Uwaga ! trzeba kasować w STML1xx w odróżnieniu od STMF4 

  DMA2->IFCR = DMA_ISR_TCIF5;          

  DMA2_Channel5->CNDTR = Count;
  DMA2_Channel5->CMAR  = (unsigned int)TxBuffer;
  
  DMA2->IFCR = DMA_ISR_TCIF5;   /*!< Channel 5 Transfer Complete flag */
  status = UART4->SR;

  SET_BIT(DMA2_Channel5->CCR, DMA_CCR5_EN); // wlacz DMA TX
  SET_BIT(UART4->CR1, USART_CR1_TCIE);
}


/**
 *  USART1_DMA_xxxFrame obsługa ELTELNET jako SLAVE 
 */
void USART1_DMA_ReceiveFrame(void *RxBuffer, unsigned char Count)
{
//  NVIC_DisableIRQ(DMA1_Channel5_IRQn);
//  CLEAR_BIT(DMA1_Channel5->CCR, DMA_CCR5_EN);  
//
//  WRITE_REG(DMA1->IFCR, DMA_IFCR_CTCIF5 | DMA_IFCR_CTEIF5);
//  NVIC_ClearPendingIRQ(DMA1_Channel5_IRQn);
//
//  DMA1_Channel5->CNDTR = Count;
//  DMA1_Channel5->CMAR  = (unsigned int)RxBuffer;
//  
//  NVIC_EnableIRQ(DMA1_Channel5_IRQn);
//  SET_BIT(DMA1_Channel5->CCR, DMA_CCR5_EN);
}

void USART1_DMA_SendFrame(void *TxBuffer, unsigned short Count)
{
//  unsigned short status;
//
//  USART1_TRANSMIT;
//  CLEAR_BIT(DMA1_Channel4->CCR, DMA_CCR4_EN);   /// Uwaga ! trzeba kasować w STML1xx w odróżnieniu od STMF4 
//
//  DMA1->IFCR = DMA_ISR_TCIF4;          
//
//  DMA1_Channel4->CNDTR = Count;
//  DMA1_Channel4->CMAR  = (unsigned int)TxBuffer;
//  
//  DMA1->IFCR = DMA_ISR_TCIF4;   /*!< Channel 4 Transfer Complete flag */
//  status = USART1->SR;
//
//  SET_BIT(DMA1_Channel4->CCR, DMA_CCR4_EN); // wlacz DMA TX
//  SET_BIT(USART1->CR1, USART_CR1_TCIE);
}

/**
 *  SPI_2_DMA_SendFrame obsługa FLASH 
 */
void SPI_2_DMA_SendFrame(void *TxBuffer, void *RxBuffer, unsigned short Count, unsigned char Device)
{
  WRITE_REG(DMA1->IFCR, DMA_IFCR_CTCIF4); // wyczysc flage RX
  WRITE_REG(DMA1->IFCR, DMA_IFCR_CTCIF5); // wyczysc flage TX

  CLEAR_BIT(DMA1_Channel4->CCR, DMA_CCR4_EN);   /// Uwaga ! trzeba kasować w STML1xx w odróżnieniu od STMF4 <jesli chcemy zmienic zawartość rejestrow DMA>
  CLEAR_BIT(DMA1_Channel5->CCR, DMA_CCR5_EN);   /// Uwaga ! trzeba kasować w STML1xx w odróżnieniu od STMF4 

  switch(Device)
  {
    case SPI2_DEVICE_LCD:     LCD_CS_0;   break;
    case SPI2_DEVICE_FLASH:   FLASH_CS_0; break;
  }

  if(RxBuffer == NULL) 
    MODIFY_REG(DMA1_Channel4->CCR, DMA_CCR4_MINC, 0);
  else 
    MODIFY_REG(DMA1_Channel4->CCR, DMA_CCR4_MINC, DMA_CCR4_MINC);

  DMA1_Channel4->CNDTR = Count;
  DMA1_Channel4->CMAR = (unsigned int)RxBuffer;
  
  DMA1_Channel5->CNDTR = Count;
  DMA1_Channel5->CMAR = (unsigned int)TxBuffer;
  
  sSPI2.Status.DMAComplete = 0;
  
  SET_BIT(DMA1_Channel4->CCR, DMA_CCR4_EN); // wlacz DMA RX
  SET_BIT(DMA1_Channel5->CCR, DMA_CCR5_EN); // wlacz DMA TX
}


/// SPI1 RX (Flash SST)
//void DMA1_Channel4_IRQHandler(void) 
//{
//  sSPI2.Status.DMAComplete = 1;
//  
//  LCD_CS_1;
//  FLASH_CS_1;
//
//  DMA1->IFCR = DMA_IFCR_CTCIF4;
//  NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
//}

/// SPI1 TX (Flash SST)
//void DMA1_Channel5_IRQHandler(void) 
//{
//  DMA1->IFCR = DMA_IFCR_CTCIF5;
//  NVIC_ClearPendingIRQ(DMA1_Channel5_IRQn);
//}
