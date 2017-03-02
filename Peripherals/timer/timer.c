#include "timer.h"
#include "stm32F10x.h"
#include "stm32F10x_tim.h"
#include "stm32F10x_rcc.h"

#include "main.h"

#include "..\StdPeriph_Driver\inc\misc.h"

void Timer_Handler(void);
void TimerLowSpeed_Handler(void);
void TimerUS_Handler(void);

signed int Timer_us[eNumberOfUSTimers];        // 100us
signed int Timer_ms[eNumberOfTimers];          // 1ms
signed int Timer_ls[eNumberOfLowSpeedTimers];  // 100ms

unsigned int TimerCS;

//--- WYKORZYSTANIE TIMER�W -----------------------------------------------------
//
//  TIM2  - Odmierzanie 1ms
//  TIM7  - Odmierzanie 100ms
//
//  TIM4  - Do wyzwalania (TRIGGER) ADC;
//
//  TIM3  - PWM CH1 pin PC6 do generowania SYGNALOW porozumiewawczych
//  TIM5  - PWM Odmierzanie 2ms
//
//  TIM9  - TR1 Timer do transmisji oraz Timeout-Synchronizacja
//
//  TIM10 - TR2 Timeout-Synchronizacja
//  TIM11 - TR2 Timer do transmisji
//
//  TIM6  - Transmisja z AKU
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
  WRITE_REG(TIM2->ARR, 0x7D00);  //
  WRITE_REG(TIM2->PSC, 0x00); 	// /(Value +1) = 1 - prescaler wy��czony

  SET_BIT(TIM2->DIER, TIM_DIER_UIE);         // wlacz przerwanie przepelnienia licznika
  SET_BIT(TIM2->CR1,  TIM_CR1_CEN);          // wlacz timer; bit DIR domyslnie 0 zliczanie w gore


  for(i = 0; i < eNumberOfTimers; i++) Timer_ms[i] = TIMER_OFF;
}

/** 
 *  \brief Init Timer 100ms
 */
void Init_LowSpeedTimer(void) // Timer ms
{
  unsigned char i;

  NVIC_InitTypeDef NVICC;
  NVICC.NVIC_IRQChannel = TIM3_IRQn;
  NVICC.NVIC_IRQChannelPreemptionPriority = 8;
  NVICC.NVIC_IRQChannelSubPriority = 0;
  NVICC.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVICC);

  NVIC_ClearPendingIRQ(TIM3_IRQn);
  NVIC_EnableIRQ(TIM3_IRQn);
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
  
  WRITE_REG(TIM7->CNT, 0x0000);
  WRITE_REG(TIM7->ARR, 0x3200); //  100ms -> 0x2E18
  WRITE_REG(TIM7->PSC, 249); // Prescaler: dzielenie przez 250

  SET_BIT(TIM7->DIER, TIM_DIER_UIE);
  SET_BIT(TIM7->CR1,  TIM_CR1_CEN); 


  for(i = 0; i < eNumberOfLowSpeedTimers; i++)
  {
    Timer_ls[i] = TIMER_OFF;
  }
}



/** 
 *  \brief Timer_Handler 1ms
 */
void Timer_Handler(void)
{  
	if(Timer_ms[eTimer_Test] > 0)					Timer_ms[eTimer_Test]--;
	if(Timer_ms[eTimer_Setup] > 0)					Timer_ms[eTimer_Setup]--;
	if(Timer_ms[eTimer_Potentiometer_Check] > 0)	Timer_ms[eTimer_Potentiometer_Check]--;
	if(Timer_ms[eTimer_Adc] > 0)					Timer_ms[eTimer_Adc]--;
	if(Timer_ms[eTimer_InterruptConnection]> 0)		Timer_ms[eTimer_InterruptConnection]--;
	if(Timer_ms[eTimer_AppsTick] > 0)				Timer_ms[eTimer_AppsTick]--;

	if(TimerCS < 0xFFFFFFFF) TimerCS++;
}

/** 
 *  \brief TimerLowSpeed_Handler 100ms
 */
void TimerLowSpeed_Handler(void)
{
  if(Timer_ls[eTimerLowSpeed_StartUp] > 0)          Timer_ls[eTimerLowSpeed_StartUp]--;
  if(Timer_ls[eTimerLowSpeed_Test] > 0)     		Timer_ls[eTimerLowSpeed_Test]--;
  if(Timer_ls[eTimerLowSpeed_Led] > 0)       		Timer_ls[eTimerLowSpeed_Led]--;
  if(Timer_ls[eTimerLowSpeed_Reset] > 0)            Timer_ls[eTimerLowSpeed_Reset]--;
  if(Timer_ls[eTimerLowSpeed_TimeDelayOn] > 0)		Timer_ls[eTimerLowSpeed_TimeDelayOn]--;
}

/** 
 *  \brief TimerUS_Handler 100us
 */
void TimerUS_Handler(void)
{
//  if(Timer_us[eTimerUS_Adc] > 0)               Timer_us[eTimerUS_Adc]--;
  if(Timer_us[eTimerUS_Dac] > 0)               Timer_us[eTimerUS_Dac]--;
}


/** 
 *  \brief Init Timer ADC Trigger
 */
void Init_FastSpeedTimer(void) 
{
  NVIC_InitTypeDef NVICC;
  
  /*!< @ref IRQn_Type IRQ Channels list, please refer to stm32l1xx.h line:212*/
  NVICC.NVIC_IRQChannel = TIM4_IRQn;

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

  NVIC_ClearPendingIRQ(TIM4_IRQn );
  NVIC_EnableIRQ(TIM4_IRQn);

  /* Enable TIM4 clocks */
  SET_BIT(RCC->APB1ENR , RCC_APB1ENR_TIM4EN); // wlacz zegar dla timera 1ms
  
  WRITE_REG(TIM4->CNT, 0x0000);
  WRITE_REG(TIM4->ARR, 0x09C4);   // 78,125us
//  WRITE_REG(TIM4->ARR, 0x0C80);   // 100us
  WRITE_REG(TIM4->PSC, 0x00);     // 
 
    /* Select the Master Slave Mode */
  TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);

  /* Master Mode selection: TIM4 */
//  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Enable);
  
  // MMS: Master mode selection
  // 010: Update - The update event is selected as trigger output (TRGO). For instance a master
  //      timer can then be used as a prescaler for a slave timer.
  MODIFY_REG(TIM4->CR2,  TIM_CR2_MMS, TIM_CR2_MMS_1); 

  SET_BIT(TIM4->DIER, TIM_DIER_UIE);         // wlacz przerwanie przepelnienia licznika
  SET_BIT(TIM4->CR1,  TIM_CR1_CEN);          // wlacz timer; bit DIR domyslnie 0 zliczanie w gore

}


/******************************************************************************/
/*            STM32L1xx Peripherals Interrupt Handlers                        */
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
  
  /* Clear TIM2 update interrupt */
//  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  

  CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
  NVIC_ClearPendingIRQ(TIM2_IRQn);
}

/**
  * @brief    This function handles TIM7 global interrupt request.
  * @param    None
  * @retval   None
  * @details  TIM7 - timer to counts 100ms
  */
void TIM7_IRQHandler(void) // timer 100ms
{
  TimerLowSpeed_Handler();
  
//  Frequency_Counter.ChannelPositive = (TIM8->CNT * 10);
//  TIM8->CNT = 0;
//  Frequency_Counter.ChannelNegative = (TIM5->CNT * 10);
//  TIM5->CNT = 0;
  
  CLEAR_BIT(TIM7->SR, TIM_SR_UIF);
  NVIC_ClearPendingIRQ(TIM3_IRQn);
}

/**
  * @brief    This function handles TIM2 global interrupt request.
  * @param    None
  * @retval   None
  * @details  TIM4 - timer to trigger ADC
  */
void TIM4_IRQHandler(void)
{
  TimerUS_Handler();

  CLEAR_BIT(TIM4->SR, TIM_SR_UIF);
  NVIC_ClearPendingIRQ(TIM4_IRQn);
}
