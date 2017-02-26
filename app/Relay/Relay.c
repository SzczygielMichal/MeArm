#include "peripherals\timer\timer.h"
#include "board\board.h"
#include "cmsis_lib\include\stm32l1xx_gpio.h"

void Relay_Trig(void)
{
	static char x = 0;

	if(TimerLowSpeedStatus(eTimerLowSpeed_Test) == TIMER_END)
	{
		TimerLowSpeedStart(eTimerLowSpeed_Test, 10); // Timer 100ms
		if(x == 0)
		{
			LED_OFF;
			x = 1;
		}
		else
		{
			LED_ON;
			x = 0;
		}
	}
}
