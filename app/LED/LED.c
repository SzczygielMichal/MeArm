#include "..\peripherals\timer\timer.h"
#include "..\board\board.h"
#include "..\StdPeriph_Driver\inc\stm32f10x_gpio.h"

void Led_Trig(void)
{
	static char x = 0;

	if(TimerStatus(eTimer_LED) == TIMER_END)
	{
		TimerStart(eTimer_LED, 1000); // Timer 100ms
		if(!x)
		{
			LED_OFF;
			x = 1;
		}
		else
		{
			LED_ON;
			x = 0;
		}
		while (!(USART3->SR & USART_SR_TC));
			USART3->DR = 'M';

	}
}
