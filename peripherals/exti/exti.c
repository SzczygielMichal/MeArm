

#include "stm32f10x.h"

#include "..\board\board.h"
#include "..\peripherals\timer\timer.h"

unsigned char CMX_FIFO_ReadIRQ;

void InitEXTI(void)
{
//  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);  /// System configuration controller clock enabled
//
///// CMX
//  MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI1, 0); // 0- PA1 SYSCFG_EXTICR1_EXTI1_PA
//  SET_BIT(EXTI->FTSR, EXTI_FTSR_TR1);
//  SET_BIT(EXTI->IMR,  EXTI_IMR_MR1);
//  SET_BIT(EXTI->EMR,  EXTI_EMR_MR1);
//  NVIC_ClearPendingIRQ(EXTI1_IRQn);
  
///// Transmisja z AKU
//  MODIFY_REG(SYSCFG->EXTICR[2], SYSCFG_EXTICR3_EXTI8, SYSCFG_EXTICR3_EXTI8_PC); // PC8 SYSCFG_EXTICRx_EXTIy_PC
//  SET_BIT(EXTI->FTSR, EXTI_FTSR_TR8);   /// Ustawiam zbocze opadajace
//  SET_BIT(EXTI->IMR,  EXTI_IMR_MR8);    /// Interrupt z linii nie jest maskowane
//  SET_BIT(EXTI->EMR,  EXTI_EMR_MR8);    /// Event     z linii nie jest maskowane
//  NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
//
//  NVIC_EnableIRQ(EXTI9_5_IRQn);
}


void Enable_EXTI(unsigned char On)
{
  if(On == ENABLE) 
    NVIC_EnableIRQ(EXTI1_IRQn); 
  else 
    NVIC_DisableIRQ(EXTI1_IRQn);
}


/// CMX pin
void EXTI1_IRQHandler(void)
{
  CMX_FIFO_ReadIRQ = 1;

  SET_BIT(EXTI->PR,  EXTI_PR_PR1);
  NVIC_ClearPendingIRQ(EXTI1_IRQn);
}

/// AKU pin
void EXTI9_5_IRQHandler(void)
{
//  Acu_DetectFallingEdge();

  SET_BIT(EXTI->PR,  EXTI_PR_PR8);
  NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
}




