/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

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
#include "main.h"
#include "dbg_ctrl.h"

#include "..\setup\setup.h"
#include "..\board\board.h"
#include "..\board\init.h"
#include "..\app\led\LED.h"
#include "..\app\debug\debug.h"
#include "..\app\Potentiometer\Potentiometer.h"
#include "..\peripherals\iwdg\iwdg.h"
#include "..\peripherals\timer\timer.h"
#include "..\peripherals\uart\uart.h"

char buffer[100];
unsigned int x = 1000;
char op = 0;
char dir = 0;
unsigned int krok;
unsigned int we;
unsigned char wynik;
struct _sMeArm sMeArm;

int main(void)
{
	//
	// Zmienne globalne
	//

	setvbuf(stdout, NULL, _IONBF, 0); // Wy³¹czenie buforowania tekstu drukowanego przez printf

	Init_RCC();
	Init_IO();
	Init_Peripherals();

//	TimerStart(eTimer_LED, 10);			// Miganie LED6 co 1s
	TimerStart(eTimer_Key, 500);			// Miganie LED6 co 1s

	Init_Debug();
	Debug_Intro();

	while(1)
	{
		IWDG_Reset();						// Waruj - reset watchdoga
//	    Led_Trig();
	    Debug_Handler();					// Obsluga konsoli debugowania
//	    Potentiometer_Handler();			// Odczyt nastaw potencjometrów

/*		if(TimerStatus(eTimer_Servo_1) == TIMER_END)
		{
			TimerStart(eTimer_Servo_1, 1000); // Timer 100ms
			if(dir)
			{
				TIM4->CCR1 = 970;
				TIM4->CCR2 = 970;
				TIM4->CCR3 = 970;
				TIM4->CCR4 = 970;
				dir = 0;
			}
			else
			{
				TIM4->CCR1 = 870;
				TIM4->CCR2 = 870;
				TIM4->CCR3 = 870;
				TIM4->CCR4 = 870;
				dir = 1;
				krok++;
			}
		}
*/
	}
	return 0;
}

void Debug_Intro (void)
{
	printf("\033[2J");    // Czyszczenie ekranu
	printf("\033[00f");  // Ustawienie kursora
	printf("********************************************************************\n\r");
	printf("********                                                    ********\n\r");
	printf("********                 Micha³ Szczygie³                   ********\n\r");
	printf("********                PCB TEST STM32F103                  ********\n\r");
	printf("********                                                    ********\n\r");
	printf("********************************************************************\n\r");
	printf("********                       \033[33mMeARM\033[39m                        ********\n\r");
	printf("********************************************************************\n\r");
//	printf("********        Data kompilacji:      %14s        ********\n\r", DATE_NOW);
//	printf("********        Godzina kompilacji:   %14s        ********\n\r", TIME_NOW);
//	printf("********        Data rewizji:         %14s        ********\n\r", DATE_REV);
//	printf("********        Godzina rewizji:      %14s        ********\n\r", TIME_REV);
//	printf("********        Rewizja:              %14d        ********\n\r", REVISION);
	printf("********        Wersja Pcb:           %14s        ********\n\r", BOARDREV);
	printf("********************************************************************\n\r");
}
