#include "board\board.h"
#include "stm32l1xx.h"
#include "cmsis_lib\include\stm32l1xx_gpio.h"
#include "peripherals\timer\timer.h"
#include "app\ptc\ptc.h"
#include "setup\setup.h"

void TimeDelayOn_Handler(void)
{
	static char ustawiony = 0;

	if(TimerStatus(eTimer_TimeDelayOnCheck) == TIMER_END)
	{
		TimerStart(eTimer_TimeDelayOnCheck, 50);

		if((CHECK_TIME_LINE == 1) && (ustawiony == 0))
		{
			TimerLowSpeedStart(eTimerLowSpeed_TimeDelayOn, pk_elS.Setup.TimeDelayOn*10);	// Odmierzanie czasu do włączenia przekaźnika REL5
			ustawiony = 1;
		}

		if((CHECK_TIME_LINE == 0) && (ustawiony == 1))
		{
			TimerLowSpeedStop(eTimerLowSpeed_TimeDelayOn);
			RELAY_5_OFF;
			ustawiony = 0;
		}
	}

	if(TimerLowSpeedStatus(eTimerLowSpeed_TimeDelayOn) == TIMER_END)
	{
		TimerLowSpeedStop(eTimerLowSpeed_TimeDelayOn);
		RELAY_5_ON;
	}
}
