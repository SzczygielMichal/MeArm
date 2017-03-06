//---------------------------------------------------
//                   spi.c
//---------------------------------------------------

#include "..\board\board.h"
#include "stm32f10x.h"
#include "stddef.h"

#include "spi.h"
#include "main.h"

struct stSPI0 SPI_1; // spi na przerwaniach
struct stSPI1 SPI_2; // spi na przerwaniach

void SPI1_IRQHandler(void)
{
  SPI_1_Handler();

  CLEAR_BIT(SPI1->SR, SPI_SR_RXNE);
  NVIC_ClearPendingIRQ(SPI1_IRQn);
}


void ClearWhatever(void *Ptr, unsigned short Count)
{
  unsigned short i;
  for(i = 0; i < Count; i++) 
  {
    *((unsigned char*)Ptr++) = 0;
  }
}

void SPI_1_Handler(void)
{
//  unsigned short Status;

//  Status = SPI1->SR;
  
  //----- TX ------------------
  if(SPI_1.TxCounter < SPI_1.TxCount)
  {
    SPI1->DR = SPI_1.TxBuffer[SPI_1.TxCounter++];
  }
  else
  {
    SPI_1.Status.TxComplete = 1;
    SPI_1.Status.Busy = 0;
    if(SPI_1.afterTx != NULL) 
    {
      SPI_1.afterTx();
      SPI_1.afterTx = NULL;
    }
  }
  //----- RX ------------------
  if(SPI_1.RxCounter < SPI_1.RxCount)
  {
    SPI_1.RxBuffer[SPI_1.RxCounter++] = SPI1->DR;
    if(SPI_1.RxCounter == SPI_1.RxCount)
    {
      SPI_1.Status.RxComplete = 1;
      SPI_1.Status.Busy = 0;
      if(SPI_1.afterRx != NULL)
      {
        SPI_1.afterRx();
        SPI_1.afterRx = NULL;
      }
    }
  }else SPI_1.RxCounter = 0;
}


void SPI_1_ChangeBitFormat(unsigned char BitFormat)
{
  while(SPI1->SR & SPI_SR_BSY){}
  CLEAR_BIT(SPI1->CR1, SPI_CR1_SPE);
  
  CLEAR_BIT(DMA1_Channel2->CCR, DMA_CCR2_EN);
  CLEAR_BIT(DMA1_Channel3->CCR, DMA_CCR3_EN);

  if(BitFormat == 16) 
  {
    SET_BIT(SPI1->CR1, SPI_CR1_DFF);
    MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_MSIZE, DMA_CCR2_MSIZE_0); // 16 bit MSIZE i PSIZE  (RX)
    MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_PSIZE, DMA_CCR2_PSIZE_0); // 16 bit MSIZE i PSIZE

    MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_MSIZE, DMA_CCR3_MSIZE_0); // 16 bit MSIZE i PSIZE  (TX)
    MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_PSIZE, DMA_CCR3_PSIZE_0); // 16 bit MSIZE i PSIZE
  }
  else 
  {
    CLEAR_BIT(SPI1->CR1, SPI_CR1_DFF);
    MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_MSIZE, 0); // 8 bit MSIZE i PSIZE  (RX)
    MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_PSIZE, 0); // 8 bit MSIZE i PSIZE

    MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_MSIZE, 0); // 8 bit MSIZE i PSIZE  (TX)
    MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_PSIZE, 0); // 8 bit MSIZE i PSIZE
  }
  SET_BIT(SPI1->CR1, SPI_CR1_SPE);
  SET_BIT(DMA1_Channel2->CCR, DMA_CCR2_EN);
  SET_BIT(DMA1_Channel3->CCR, DMA_CCR3_EN);

  while(SPI1->SR & SPI_SR_BSY){}
}



void SPI_1_DMA_Mode_Init(void)
{
  NVIC_InitTypeDef NVICC;
  NVICC.NVIC_IRQChannel = DMA1_Channel2_IRQn;//DMA1_Channel1_IRQn
  NVICC.NVIC_IRQChannelPreemptionPriority = 1;
  NVICC.NVIC_IRQChannelSubPriority = 0;
  NVICC.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVICC);
  
  CLEAR_BIT(RCC->APB2ENR,  RCC_APB2ENR_SPI1EN);
  SET_BIT(  RCC->APB2RSTR, RCC_APB2RSTR_SPI1RST); // reset
  CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SPI1RST); // reset
  SET_BIT(  RCC->APB2ENR,  RCC_APB2ENR_SPI1EN);
   
  SET_BIT(SPI1->CR1, SPI_CR1_CPHA);
  SET_BIT(SPI1->CR1, SPI_CR1_CPOL);
  SET_BIT(SPI1->CR1, SPI_CR1_MSTR);
  SET_BIT(SPI1->CR1, SPI_CR1_SSM);
  SET_BIT(SPI1->CR1, SPI_CR1_SSI);
  
  MODIFY_REG(SPI1->CR1, SPI_CR1_BR, SPI_CR1_BR_0); /// przez 4 = 14,74MHz/4 = 3686400Hz ///SPI_CR1_BR_2  ); // przez 32

  //SET_BIT(SPI1->CR2, SPI_CR2_SSOE);           // SS Output Enable 
  SET_BIT(SPI1->CR2, SPI_CR2_TXDMAEN);        // kontrola przez DMA
  SET_BIT(SPI1->CR2, SPI_CR2_RXDMAEN);        // kontrola przez DMA

  SET_BIT(RCC->AHBENR, RCC_AHBENR_DMA1EN);    // wlacz taktowanie dla DMA
 /// STm32L1xx inaczej niz w F4 nie ma Stream-ów a podlaczene do peryferiow 

/**
 *  Konfiguracja kanalow DMA
 */
  WRITE_REG(DMA1_Channel2->CPAR, (unsigned int)&SPI1->DR); // RX
  WRITE_REG(DMA1_Channel3->CPAR, (unsigned int)&SPI1->DR); // TX


  MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_PL, 0 );  // Priorytet kanału
  MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_PL, 0 );

  MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_MINC, DMA_CCR2_MINC);    // inkrementacja pamieci
  MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_MINC, DMA_CCR3_MINC);    // inkrementacja pamieci

  MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_DIR, 0           );      // peryferium -> pamiec
  MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_DIR, DMA_CCR3_DIR);      // pamiec -> peryferium
  

  MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_TCIE, DMA_CCR2_TCIE );  // wl/ przerwanie transfer complete

  NVIC_ClearPendingIRQ(DMA1_Channel2_IRQn);
  NVIC_EnableIRQ(DMA1_Channel2_IRQn);


  SET_BIT(SPI1->CR1, SPI_CR1_SPE);  // SPI Enable
  while(SPI1->SR & SPI_SR_BSY){}
}


/// SPI_2

void SPI_2_DMA_Mode_Init(void)
{
  NVIC_InitTypeDef NVICC;
  NVICC.NVIC_IRQChannel = DMA1_Channel2_IRQn;//DMA1_Channel1_IRQn
  NVICC.NVIC_IRQChannelPreemptionPriority = 1;
  NVICC.NVIC_IRQChannelSubPriority = 0;
  NVICC.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVICC);
  
  CLEAR_BIT(RCC->APB2ENR,  RCC_APB2ENR_SPI1EN);
  SET_BIT(  RCC->APB2RSTR, RCC_APB2RSTR_SPI1RST); // reset
  CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SPI1RST); // reset
  SET_BIT(  RCC->APB2ENR,  RCC_APB2ENR_SPI1EN);
   
  SET_BIT(SPI1->CR1, SPI_CR1_CPHA);
  SET_BIT(SPI1->CR1, SPI_CR1_CPOL);
  SET_BIT(SPI1->CR1, SPI_CR1_MSTR);
  SET_BIT(SPI1->CR1, SPI_CR1_SSM);
  SET_BIT(SPI1->CR1, SPI_CR1_SSI);
  
  MODIFY_REG(SPI1->CR1, SPI_CR1_BR, SPI_CR1_BR_2  ); // przez 32

  //SET_BIT(SPI1->CR2, SPI_CR2_SSOE);           // SS Output Enable 
  SET_BIT(SPI1->CR2, SPI_CR2_TXDMAEN);        // kontrola przez DMA
  SET_BIT(SPI1->CR2, SPI_CR2_RXDMAEN);        // kontrola przez DMA

  SET_BIT(RCC->AHBENR, RCC_AHBENR_DMA1EN);    // wlacz taktowanie dla DMA
 /// STm32L1xx inaczej niz w F4 nie ma Stream-ów a podlaczene do peryferiow 

/**
 *  Konfiguracja kanalow DMA
 */
  WRITE_REG(DMA1_Channel2->CPAR, (unsigned int)&SPI1->DR); // RX
  WRITE_REG(DMA1_Channel3->CPAR, (unsigned int)&SPI1->DR); // TX


  MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_PL, 0 );  // Priorytet kanału
  MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_PL, 0 );

  MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_MINC, DMA_CCR2_MINC);    // inkrementacja pamieci
  MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_MINC, DMA_CCR3_MINC);    // inkrementacja pamieci

  MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_DIR, 0           );      // peryferium -> pamiec
  MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR3_DIR, DMA_CCR3_DIR);      // pamiec -> peryferium
  

  MODIFY_REG(DMA1_Channel2->CCR, DMA_CCR2_TCIE, DMA_CCR2_TCIE );  // wl/ przerwanie transfer complete

  NVIC_ClearPendingIRQ(DMA1_Channel2_IRQn);
  NVIC_EnableIRQ(DMA1_Channel2_IRQn);


  SET_BIT(SPI1->CR1, SPI_CR1_SPE);  // SPI Enable
  while(SPI1->SR & SPI_SR_BSY){}
}

void SPI_2_Handler(void)
{
//  unsigned short Status;

//	Status = SPI2->SR;
  
  //----- TX ------------------
  if(SPI_2.TxCounter < SPI_2.TxCount)
  {
    SPI2->DR = SPI_2.TxBuffer[SPI_2.TxCounter++];
  }
  else
  {
    SPI_2.Status.TxComplete = 1;
    SPI_2.Status.Busy = 0;
    if(SPI_2.afterTx != NULL) 
    {
      SPI_2.afterTx();
      SPI_2.afterTx = NULL;
    }
  }
  //----- RX ------------------
  if(SPI_2.RxCounter < SPI_2.RxCount)
  {
    SPI_2.RxBuffer[SPI_2.RxCounter++] = SPI2->DR;
    if(SPI_2.RxCounter == SPI_2.RxCount)
    {
      SPI_2.Status.RxComplete = 1;
      SPI_2.Status.Busy = 0;
      if(SPI_2.afterRx != NULL)
      {
        SPI_2.afterRx();
        SPI_2.afterRx = NULL;
      }
    }
  }else SPI_2.RxCounter = 0;  
}

void SPI2_IRQHandler(void)
{
  SPI_2_Handler();

  CLEAR_BIT(SPI2->SR, SPI_SR_RXNE);
  NVIC_ClearPendingIRQ(SPI2_IRQn);
}

unsigned char SPI_2_CheckBusy(void)
{
  if(SPI_2.Status.Busy == 1) return F_RESULT_OK; else return F_RESULT_ERR;
}

