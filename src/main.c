/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <board.h>
#include <init.h>
#include "stm32f10x.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "syscalls.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dac.h"
#include "..\setup\setup.h"
#include "..\app\debug\debug.h"
#include "..\app\led\led.h"
//#include "app\version\version.h"
#include "..\app\potentiometer\potentiometer.h"
#include "dbg_ctrl.h"
#include "main.h"
#include "../peripherals/iwdg/iwdg.h"
#include "../peripherals/timer/timer.h"
#include "../peripherals/uart/uart.h"

char buffer[100];
unsigned char x;
char op;
unsigned int we;
unsigned char wynik;
struct _sMeArm sMeArm;

int main(void)
{
	//
	// Zmienne globalne
	//
	op = 0;
	setvbuf(stdout, NULL, _IONBF, 0); // Wyï¿½ï¿½czenie buforowania  tekstu drukowanego przez printf

	StartUpReason();

	Init_RCC();
	Init_IO();
//	Init_Peripherals();
//	Init_ADC();
//	Init_Setup();
//	Init_DAC();

//	Short_Circuit_BlockadeE_Delay_Reset_Handler();

//	TimerLowSpeedStart(eTimerLowSpeed_Led, 10);			// Miganie LED6 co 1s
//	TimerStart(eTimer_Setup, 100);						// Sprawdzanie
//	TimerStart(eTimer_PTC_Check, 50);					// Sprawdzanie temperatury PTC co 50ms
//	TimerStart(eTimer_TimeDelayOnCheck, 50);			// Odmierzanie czasu do wÅ‚Ä…czenia przekaÅºnika REL5
//	TimerStart(eTimer_Potentiometer_Check, 50);
//	TimerStart(eTimer_ShortCircuit_Check, 50);
//	TimerUStart(eTimerUS_Dac, 40);

//	Init_Debug();
//	Init_IWDG();

//	Debug_Intro();

	while(1)
	{
//		IWDG_Reset();						// Waruj - reset watch doga
//	    Led_Trig();
//	    PTC_Handler();						// Obs³‚uga wejcia czujnika temperatury
//	    Debug_Handler();					// Obsluga konsoli debugowania
//	    Potentiometer_Handler();			// Odczyt nastaw potencjometrów
//	    TimeDelayOn_Handler();				// W³¹czenie przzekaŸnika po odliczonym czasie
//	    Short_Circuit_Blockade_Handler();	// Blokada doziemienia
//	    DAC_Handler();
	}
	return 0;
}

//void Debug_Intro (void)
//{
//	printf("\033[2J");    // Czyszczenie ekranu
//	printf("\033[00f");  // Ustawienie kursora
//	printf("********************************************************************\n\r");
//	printf("********                  Eltel Sp. z o.o.                  ********\n\r");
//	printf("********          Katowice 40-312 ul.Milowicka 1E           ********\n\r");
//	printf("********             www.eltel.katowice.pl                  ********\n\r");
//	printf("********                  tel. 32-202-78-86                 ********\n\r");
//	printf("********************************************************************\n\r");
//	printf("********             \033[33mPrzekaÅºnik elektroniczny\033[39m               ********\n\r");
//	printf("********************************************************************\n\r");
//	printf("********        Data kompilacji:      %14s        ********\n\r", DATE_NOW);
//	printf("********        Godzina kompilacji:   %14s        ********\n\r", TIME_NOW);
//	printf("********        Data rewizji:         %14s        ********\n\r", DATE_REV);
//	printf("********        Godzina rewizji:      %14s        ********\n\r", TIME_REV);
//	printf("********        Rewizja:              %14d        ********\n\r", REVISION);
//	printf("********        Wersja Pcb:           %14s        ********\n\r", BOARDREV);
//	printf("********************************************************************\n\r");
//}
