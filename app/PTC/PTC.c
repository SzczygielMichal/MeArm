#include "board\board.h"
#include "stm32l1xx.h"
#include "board\init.h"
#include "setup\setup.h"
#include "cmsis_lib\include\stm32l1xx_gpio.h"
#include "peripherals\timer\timer.h"

void PTC_Handler(void)
{
	static char ustawiony = 0;

	if(TimerStatus(eTimer_PTC_Check) == TIMER_END)
	{
		TimerStart(eTimer_PTC_Check,50); // Sprawdzanie temperatury PTC co 50ms
		if((CHECK_PTC_LINE == 1) && (ustawiony == 0))
		{
			if(pk_elS.Setup.PTC_Control)
				TimerStart(eTimer_PTC_Delay, 500);
			else
				TimerStart(eTimer_PTC_Delay, 100);
			ustawiony = 1;
		}
		else if((CHECK_PTC_LINE == 0) && (ustawiony == 1))
		{
			RELAY_1_ON;
			ustawiony = 0;
		}
	};

	if(TimerStatus(eTimer_PTC_Delay) == TIMER_END)
	{
		TimerStop(eTimer_PTC_Delay);
		RELAY_1_OFF;
	}
}
