

//  #include "main.h"
#include "stm32l1xx_iwdg.h"
void Init_IWDG(void);
void IWDG_Reset(void);


void Init_IWDG(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  IWDG_SetPrescaler(IWDG_Prescaler_16);
  
  IWDG_SetReload(0xFFF);
  
  IWDG_ReloadCounter();
  
  IWDG_Enable();
}


/* Resetowanie WatchDog - zanim on zresetuje nas :) */
void IWDG_Reset(void) 
{
  IWDG_ReloadCounter();
}





