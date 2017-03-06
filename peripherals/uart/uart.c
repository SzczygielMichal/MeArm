
#include "uart.h"

#include "main.h"
#include "stm32f10x.h"
#include "..\board\board.h"    // Date 26.02.2017
//#include "..\peripherals\dma\dma.h"
#include "..\app\debug\debug.h"
#include "..\StdPeriph_Driver\inc\misc.h"
#include "..\timer\timer.h"

struct specialUART  sUART1;


/** 
 *  TRANSMISJA Z KONSOLĄ
 */
void USART1_IRQHandler(void)
{
  unsigned int Status;
  unsigned short Data;

  Status = USART1->SR;

  if(Status & (USART_SR_ORE))
  {
    Data = USART1->DR;
    CLEAR_BIT(USART1->SR, USART_SR_RXNE);
    CLEAR_BIT(USART1->SR, USART_SR_ORE);
  }
  if(Status & USART_SR_TC)
  {
    CLEAR_BIT(USART1->SR, USART_SR_TC);
    if(sUART1.TxCounter < sUART1.TxCount)
    {
      USART1->DR = sUART1.TxBuffer[sUART1.TxCounter++];
    }
  }
  if(Status & USART_SR_RXNE)
  {
    CLEAR_BIT(USART1->SR, USART_SR_RXNE);
    Data = USART1->DR;
    Debug_UART_Rx_Handler((unsigned char)Data);
  }

  NVIC_ClearPendingIRQ(USART1_IRQn);
}
/** 
 *  TRANSMISJA Z KONSOLĄ
 */
void InitUART1(void)
{
  NVIC_InitTypeDef NVICC;
  NVICC.NVIC_IRQChannel = USART1_IRQn;
  NVICC.NVIC_IRQChannelPreemptionPriority = 3;
  NVICC.NVIC_IRQChannelSubPriority = 0;
  NVICC.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVICC);

  SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USART1RST);
  CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USART1RST);
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  MODIFY_REG(USART1->CR1, USART_CR1_TE, USART_CR1_TE);
  MODIFY_REG(USART1->CR1, USART_CR1_RE, USART_CR1_RE);
        
  MODIFY_REG(USART1->CR2, USART_CR2_STOP, USART_CR2_STOP_1);
  
  // Prędkosć 115200
//  MODIFY_REG(USART1->BRR, USART_BRR_DIV_MANTISSA, 16 << USART_BRR_DIV_Mantissa_BIT);
//  MODIFY_REG(USART1->BRR, USART_BRR_DIV_FRACTION, 16 << USART_BRR_DIV_Mantissa_BIT);
  USART1->BRR = (0x11 << 4) + 6;
  
  USART1->SR = 0;
  sUART1.RxCount = 0;
  sUART1.TxCount = 0;
  
  MODIFY_REG(USART1->CR1, USART_CR1_UE, USART_CR1_UE);
  
  NVIC_ClearPendingIRQ(USART1_IRQn);
  
  SET_BIT(USART1->CR1, USART_CR1_TCIE);
  SET_BIT(USART1->CR1, USART_CR1_RXNEIE);
  
  NVIC_EnableIRQ(USART1_IRQn);
}

/** 
 *  TRANSMISJA Z KONSOLĄ
 */
void SendUART1(void *Buffer, unsigned short Count)
{
	unsigned short i;
	unsigned char *Ptr;

	Ptr = (unsigned char*)Buffer;
	for(i = 0; i < Count; i++)
		sUART1.TxBuffer[i] = *(Ptr++);

	sUART1.TxCount = Count;
	sUART1.TxCounter = 1;
	sUART1.RxCount = 0;
	sUART1.RxCounter = 0;

	CLEAR_BIT(USART1->SR, USART_SR_TC);
	CLEAR_BIT(USART1->SR, USART_SR_TXE);
	NVIC_ClearPendingIRQ(USART1_IRQn);

	USART1->DR = sUART1.TxBuffer[0]; // rozpoczyna transmisje
}
