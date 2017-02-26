//---------------------------------------------------
//                   pwm.c
//---------------------------------------------------

#include "stm32l1xx.h"

#include "app\signal\signal.h"


void TIM5_IRQHandler(void) // timer 2ms
{
  switch(SignalGen.SignalNr)
  {
    case 2:
    switch(SignalGen.DevDirect)
    {
      case DEV_DIRECT_UP:
        SignalGen.Dev += SignalGen.DevStep;
        if(SignalGen.Dev >= SignalGen.DevMax)
        {
          SignalGen.DevDirect = DEV_DIRECT_DOWN;
        }
        else
        {
          TIM3->ARR = (unsigned short)SignalGen.Dev;
          TIM3->CCR1 = (unsigned short)SignalGen.Dev/2;
        }
        break;
      case DEV_DIRECT_DOWN:
        SignalGen.Dev -= SignalGen.DevStep;
        if(SignalGen.Dev <= SignalGen.DevMin)
        {
          SignalGen.DevDirect = DEV_DIRECT_UP;
        }
        else
        {
          TIM3->ARR = (unsigned short)SignalGen.Dev;
          TIM3->CCR1 = (unsigned short)SignalGen.Dev/2;
        }
        break;
    }
    break;
  }
  
  CLEAR_BIT(TIM5->SR, TIM_SR_UIF);
  NVIC_ClearPendingIRQ(TIM5_IRQn);
}

// generowanie sygnalu

void Init_PWM_TIM3(void)
{
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
  
  SET_BIT(TIM3->CR1, TIM_CR1_CEN);
  
  TIM3->PSC = 0;

  /**
   * PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
   * else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0) as long as
   * TIMx_CNT>TIMx_CCR1 else active (OC1REF=1).
   */
  MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_OC1M, TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
  
  /**
   * CC1 channel configured as output:
   * 0: Off - OC1 is not active
   * 1: On - OC1 signal is output on the corresponding output pin
   */
  SET_BIT(TIM3->CCER, TIM_CCER_CC1E);
  
  TIM3->CCR1 = 0;
}

void Init_TIM5(void)
{
  NVIC_ClearPendingIRQ(TIM5_IRQn);
  NVIC_EnableIRQ(TIM5_IRQn);
   
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM5EN);
  
  WRITE_REG(TIM5->CNT, 0x0000);
  WRITE_REG(TIM5->ARR, 0x4CCE);
  WRITE_REG(TIM5->PSC, 0x02);

  SET_BIT(TIM5->DIER, TIM_DIER_UIE);
}

