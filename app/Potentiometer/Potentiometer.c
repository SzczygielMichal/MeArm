#include "cmsis_lib\include\stm32l1xx_adc.h"
#include "board\init.h"
#include "setup\setup.h"
#include "peripherals\ADC\ADC.h"
#include "peripherals\timer\timer.h"
#include "app\potentiometer\potentiometer.h"


void Potentiometer_Handler(void)
{
	unsigned char krok;
	unsigned int pomiar;

	if(TimerStatus(eTimer_Potentiometer_Check) == TIMER_END)
	{
		TimerStart(eTimer_Potentiometer_Check, 50);

		// Czas załączenia przekaźnika
		pomiar = 0;

		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_24Cycles);
		for(krok=0; krok<4; krok++)
		{
			ADC_SoftwareStartConv(ADC1);
			while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
			pomiar += ADC_GetConversionValue(ADC1);
		}
		pk_elS.Setup.TimeDelayOn = pot((unsigned int **)delayTimeOnTable, pomiar>>2)-1;
		pomiar = 0;

		// Ustawienie prądu znamionowego
		ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_24Cycles);
		for(krok=0; krok<4; krok++)
		{
			ADC_SoftwareStartConv(ADC1);
			while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
			pomiar += ADC_GetConversionValue(ADC1);
		}
		pk_elS.Setup.Nominal_Current_Pot = pot((unsigned int **)nominalCcurrentTable, pomiar>>2)+6;
		pomiar = 0;

		// ustawienie prądu zwarciowego
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_24Cycles);
		for(krok=0; krok<4; krok++)
		{
			ADC_SoftwareStartConv(ADC1);
			while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
			pomiar += ADC_GetConversionValue(ADC1);
		}
		pk_elS.Setup.Short_Circuit_Current = pot((unsigned int **)shortCircuitCurrentTable, pomiar>>2)+2;
	}

	i_nom = mull*pk_elS.Setup.Nominal_Current_Pot; 							// wynik jest pomno�ony przez 10
	short_circuit_current = i_nom*pk_elS.Setup.Short_Circuit_Current;		// wynik jest pomno�ony przez 10

}
