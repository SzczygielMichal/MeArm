#include "board.h"
#include "stm32F10x.h"  /// NVIC_SystemReset

#include "../peripherals/timer/timer.h"
  

  unsigned int Build;

/**
* Przepisuje dane w postaci charów, możliwość przepisania czegokolwiek
*/
void RewriterCharBuffer(unsigned char *Dst, unsigned char *Src, unsigned short Count)
{
  unsigned short i;
  for(i = 0; i < Count; i++)
  {
    *(Dst++) = *(Src++);
  }
}

/**
* Zmiana endiana
*/
unsigned int ConvertIntEndian(unsigned int Data)
{
  unsigned int Temp = 0;
  Temp |= (Data & 0xFF000000)>> 24;
  Temp |= (Data & 0x00FF0000)>> 8;
  Temp |= (Data & 0x0000FF00)<< 8;
  Temp |= (Data & 0x000000FF)<< 24;
  return Temp;
}

/*
void Reset_TimeStart(void)
{
  TimerLowSpeedStart(eTimerLs_Reset, 10);
}
*/
/*
void Reset_Handler(void)
{
  if(TimerLowSpeedStatus(eTimerLs_Reset) == TIMER_END)
  {
//    TimerLowSpeedStop(eTimerLs_Reset);
//    ctl_global_interrupts_disable();
//    NVIC_SystemReset();
  }  
}
*/


