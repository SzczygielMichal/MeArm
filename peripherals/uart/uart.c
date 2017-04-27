
#include "uart.h"
#include "main.h"
#include "stm32f10x.h"
#include "..\board\board.h"    // Date 26.02.2017
//#include "..\peripherals\dma\dma.h"
#include "..\app\debug\debug.h"
#include "..\StdPeriph_Driver\inc\misc.h"
#include "..\timer\timer.h"

struct specialUART  sUART1, sUART3;

/** 
 *  TRANSMISJA Z KONSOL¥
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
 *  TRANSMISJA Z KONSOLÄ„
 */
void InitUART1(void) // konsola
{
	NVIC_InitTypeDef NVICC;
	NVICC.NVIC_IRQChannel = USART1_IRQn;
	NVICC.NVIC_IRQChannelPreemptionPriority = 0;
	NVICC.NVIC_IRQChannelSubPriority = 0;
	NVICC.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICC);

	SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USART1RST);
	CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USART1RST);
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	MODIFY_REG(USART1->CR1, USART_CR1_TE, USART_CR1_TE);
	MODIFY_REG(USART1->CR1, USART_CR1_RE, USART_CR1_RE);

	MODIFY_REG(USART1->CR2, USART_CR2_STOP, USART_CR2_STOP_1);

	// PrÄ™dkosÄ‡ 115200
	USART1->BRR = (0x27 << 4) + 0x0001;

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
 *  TRANSMISJA Z KONSOLÄ„
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

/**
 *  TRANSMISJA Z MODU£EM BLUETOOTH
 */
void USART3_IRQHandler(void)
{
	unsigned int Status;
	unsigned short Data, Data1;

	Status = USART3->SR;

	if(Status & (USART_SR_ORE))
	{
		Data = USART3->DR;
		CLEAR_BIT(USART3->SR, USART_SR_RXNE);
		CLEAR_BIT(USART3->SR, USART_SR_ORE);
	}
	if(Status & USART_SR_TC)
	{
		CLEAR_BIT(USART3->SR, USART_SR_TC);
		if(sUART3.TxCounter < sUART3.TxCount)
		{
			USART3->DR = sUART3.TxBuffer[sUART3.TxCounter++];
		}
	}
	if(Status & USART_SR_RXNE)
	{
		CLEAR_BIT(USART3->SR, USART_SR_RXNE);
		Data = USART3->DR;
		Data1 = Data;
	//
	//	Procedura obs³ugi pobieranych danych
	//  TR_EXT_RxHandler((unsigned char)data);  - z SAGA-EX
	//

	}

	NVIC_ClearPendingIRQ(USART3_IRQn);
}
/**
 *  TRANSMISJA Z KONSOLÄ„
 */
void InitUART3(void) // konsola
{
	NVIC_InitTypeDef NVICC;
	NVICC.NVIC_IRQChannel = USART3_IRQn;
	NVICC.NVIC_IRQChannelPreemptionPriority = 0;
	NVICC.NVIC_IRQChannelSubPriority = 0;
	NVICC.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICC);

	SET_BIT(RCC->APB1RSTR, RCC_APB1RSTR_USART3RST);
	CLEAR_BIT(RCC->APB1RSTR, RCC_APB1RSTR_USART3RST);
	RCC->APB2ENR |= RCC_APB1ENR_USART3EN;

	MODIFY_REG(USART3->CR1, USART_CR1_TE, USART_CR1_TE);
	MODIFY_REG(USART3->CR1, USART_CR1_RE, USART_CR1_RE);

	MODIFY_REG(USART3->CR2, USART_CR2_STOP, USART_CR2_STOP_1);

	// Prêdkoœc 9600 - prawdopodobnie powinno byc 38400
	//  USART3->BRR = (0x01D4 << 4) + 0x000C;

	USART3->SR = 0;
	sUART3.RxCount = 0;
	sUART3.TxCount = 0;

	MODIFY_REG(USART3->CR1, USART_CR1_UE, USART_CR1_UE);

	NVIC_ClearPendingIRQ(USART3_IRQn);

	SET_BIT(USART3->CR1, USART_CR1_TCIE);
	SET_BIT(USART3->CR1, USART_CR1_RXNEIE);

	USART_Cmd(USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // (Starts and attaches Peripheral clock for USART3)

	NVIC_EnableIRQ(USART3_IRQn);
}

void Init_USART3(int baud)
{
	//Init_USART3_RCC();
	Init_USART3_GPIO();

	USART_InitTypeDef USART3_Config;
	USART3_Config.USART_BaudRate = baud;
	USART3_Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART3_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART3_Config.USART_Parity = USART_Parity_No;
	USART3_Config.USART_StopBits = USART_StopBits_1;
	USART3_Config.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART3_Config);

	USART_Cmd(USART3,ENABLE);

	Init_USART3_Interrupt();

	//Wifi_Init();
}

void Init_USART3_RCC()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // (Starts and attaches Peripheral clock for USART3)
}

GPIO_InitTypeDef GPIOB_Config_USART3;

void Init_USART3_GPIO()
{

	//For RX Pin ---------------------------------
	GPIOB_Config_USART3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOB_Config_USART3.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOB_Config_USART3.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIOB_Config_USART3); // Saves above configuration to associated registers

	//--------------------------------------------

	//For TX Pin ---------------------------------
	GPIOB_Config_USART3.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function Push-Pull
	GPIOB_Config_USART3.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIOB_Config_USART3); // Saves above configuration to associated registers

	//--------------------------------------------
}

NVIC_InitTypeDef USART3_Interrupt_Config;
void Init_USART3_Interrupt()
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	USART3_Interrupt_Config.NVIC_IRQChannel = USART3_IRQn;
	USART3_Interrupt_Config.NVIC_IRQChannelPreemptionPriority = 0;
	USART3_Interrupt_Config.NVIC_IRQChannelSubPriority = 0;
	USART3_Interrupt_Config.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&USART3_Interrupt_Config);

	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE); // Enable USART Interrupts
	//USART_ITConfig(USART3,USART_IT_TXE, ENABLE);
}
