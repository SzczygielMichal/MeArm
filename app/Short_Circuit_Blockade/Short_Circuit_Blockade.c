#include "stm32l1xx.h"
#include "cmsis_lib\include\stm32l1xx_gpio.h"
#include "board\board.h"
#include "board\init.h"
#include "setup\setup.h"
#include "peripherals\timer\timer.h"


void Short_Circuit_Blockade_Handler(void)
{
	static char ustawiony = 0;

	if(TimerStatus(eTimer_ShortCircuit_Check) == TIMER_END)
	{
		TimerStart(eTimer_ShortCircuit_Check, 50); // Sprawdzanie temperatury PTC co 50ms
		if((CHECK_SHORT_CIRCUIT_LINE == 0) && (ustawiony == 0))
			RELAY_4_ON;

		switch(pk_elS.Setup.Short_Circuit_Blockade)
		{
			case Short_Circuit_BlockadeE_No_Reset:		// bez resetu
				if((CHECK_SHORT_CIRCUIT_LINE == 1) && (ustawiony == 0))
				{
					RELAY_4_OFF;
					ustawiony = 1;
				}
				else if((CHECK_SHORT_CIRCUIT_LINE == 0) && (ustawiony == 1))
				{
					RELAY_4_ON;
					ustawiony = 0;
				}
				break;
			case Short_Circuit_BlockadeE_Electric_Reset:	// reset elektryczny - reset po ponownym uruchomieniu
				if((CHECK_SHORT_CIRCUIT_LINE == 1) && (ustawiony == 0))
				{
					RELAY_4_OFF;
					ustawiony = 1;
				}
				break;
			case Short_Circuit_BlockadeE_Mechanical_Reset:	// reset mechaniczny - r�cznie prze�aczy� cewk� przeka�nika
				if((CHECK_SHORT_CIRCUIT_LINE == 1) && (ustawiony == 0))
				{
					RELAY_4_OFF;
					RELAY_7_ON;
					ustawiony = 1;
				}
				else if((CHECK_SHORT_CIRCUIT_LINE == 0) && (ustawiony == 1))
				{
					RELAY_4_ON;
					RELAY_7_OFF;
					ustawiony = 0;
				}
				break;

			case Short_Circuit_BlockadeE_Delay_Reset:	// reset czasowo op�niony - reset poprzez ponowne uruchomienie urz�dzenia ale ze zw�ok� czasow�
				if((CHECK_SHORT_CIRCUIT_LINE == 1) && (ustawiony == 0))
				{
					RELAY_4_OFF;

					SET_2_SEC_OUTPUT;
					GPIOA->ODR |=  (1<<2);

					ustawiony = 1;
				}
				break;
			default:
				break;
		}
	}
}

void Short_Circuit_BlockadeE_Delay_Reset_Handler(void)
{
	if(pk_elS.Setup.Short_Circuit_Blockade == Short_Circuit_BlockadeE_Delay_Reset)
		if(CHECK_2_SEC)
		{
			TimerStart(eTimer_ShortCircuit_Check, 3000);
			while(TimerStatus(eTimer_ShortCircuit_Check) != TIMER_END){};
		}

}
