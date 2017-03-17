#include "timer.h"
#include "stm32F10x.h"
#include "stm32F10x_tim.h"
#include "stm32F10x_rcc.h"
#include "main.h"
#include "..\StdPeriph_Driver\inc\misc.h"

void Timer_Handler(void);

signed int Timer_ms[eNumberOfTimers];          // 1ms

unsigned int TimerCS;

//--- WYKORZYSTANIE TIMERÓW -----------------------------------------------------
//
//  TIM2  - Odmierzanie 1ms
//
//  TIM4  - PWM CH1 - CH4 do generowania sygna³ów dla SERW
//
//-----------------------------------------------------------------------------

/** 
 *  \brief Init Timer 1ms
 */
void Init_Timer(void) 
{
  unsigned char i;

  NVIC_InitTypeDef NVICC;
  
  /*!< @ref IRQn_Type IRQ Channels list, please refer to stm32l1xx.h line:212*/
  NVICC.NVIC_IRQChannel = TIM2_IRQn;

  /*!< Specifies the pre-emption  priority for the IRQ channel
  specified in NVIC_IRQChannel. This parameter can be a value
  between 0 and 15 as described in the table @ref NVIC_Priority_Table */
  NVICC.NVIC_IRQChannelPreemptionPriority = 1;  

  /*!< Specifies the subpriority level for the IRQ channel specified
  in NVIC_IRQChannel. This parameter can be a value
  between 0 and 15 as described in the table @ref NVIC_Priority_Table */
  NVICC.NVIC_IRQChannelSubPriority = 0;         

  /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel This parameter can be set either to ENABLE or DISABLE */   
  NVICC.NVIC_IRQChannelCmd = ENABLE; 

  NVIC_Init(&NVICC);

  NVIC_ClearPendingIRQ(TIM2_IRQn );
  NVIC_EnableIRQ(TIM2_IRQn);

  /* Enable TIM2 clocks */
  SET_BIT(RCC->APB1ENR , RCC_APB1ENR_TIM2EN); // wlacz zegar dla timera 1ms

  WRITE_REG(TIM2->CNT, 0x0000);
  WRITE_REG(TIM2->ARR, 0x8CA0);		// Nale¿y wpisac wartoœc potrzebn¹ do uzyskania czasu 1ms -NIE DZIA£A DLA Fck = 72MHZ
  WRITE_REG(TIM2->PSC, 0x0001);		//(Value + 1) = 2 - prescaler

  SET_BIT(TIM2->DIER, TIM_DIER_UIE);         // wlacz przerwanie przepelnienia licznika
  SET_BIT(TIM2->CR1,  TIM_CR1_CEN);          // wlacz timer; bit DIR domyslnie 0 zliczanie w gore

  for(i = 0; i < eNumberOfTimers; i++) Timer_ms[i] = TIMER_OFF;
}

/** 
 *  \brief Init Timer for PWM
 */
void Init_PWMTimer(void) // Timer ms
{
	NVIC_InitTypeDef NVICC;

	SET_BIT(RCC->APB1ENR , RCC_APB1ENR_TIM4EN); // wlacz zegar dla timera 4

	NVICC.NVIC_IRQChannel = TIM4_IRQn;
	NVICC.NVIC_IRQChannelPreemptionPriority = 0;
	NVICC.NVIC_IRQChannelSubPriority = 0;
	NVICC.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICC);

	TIM4->ARR = 1000-1;
	TIM4->PSC = 72-1;

	TIM4->CCMR1 |= TIM_OCMode_PWM2<<8 | TIM_OCMode_PWM2;
	TIM4->CCMR2 |= TIM_OCMode_PWM2<<8 | TIM_OCMode_PWM2;
	TIM4->CCER	|= TIM_OutputState_Enable<<12 | TIM_OutputState_Enable<<8 | TIM_OutputState_Enable<<4 | TIM_OutputState_Enable;

	TIM4->CCR1 = 1;
	TIM4->CCR2 = 350;
	TIM4->CCR3 = 650;
	TIM4->CCR4 = 999;

	TIM4->CR1 |= TIM_CR1_CEN;

	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
}

/** 
 *  \brief Timer_Handler 1ms
 */
void Timer_Handler(void)
{  
	if(Timer_ms[eTimer_Test] > 0)					Timer_ms[eTimer_Test]--;
	if(Timer_ms[eTimer_Potentiometer_Check] > 0)	Timer_ms[eTimer_Potentiometer_Check]--;
	if(Timer_ms[eTimer_Adc] > 0)					Timer_ms[eTimer_Adc]--;
	if(Timer_ms[eTimer_LED] > 0)					Timer_ms[eTimer_LED]--;
	if(Timer_ms[eTimer_AppsTick] > 0)				Timer_ms[eTimer_AppsTick]--;

	if(TimerCS < 0xFFFFFFFF) TimerCS++;
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief    This function handles TIM2 global interrupt request.
  * @param    None
  * @retval   None
  * @details  TIM2 - timer to counts 1ms
  */
void TIM2_IRQHandler(void)
{
  Timer_Handler();

  CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
  NVIC_ClearPendingIRQ(TIM2_IRQn);
}
