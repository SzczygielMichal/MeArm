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
#include "dbg_ctrl.h"
#include "main.h"

#include "..\app\led\LED.h"
#include "..\app\debug\debug.h"
#include "..\app\Potentiometer\Potentiometer.h"
#include "..\peripherals\iwdg\iwdg.h"
#include "..\peripherals\timer\timer.h"
#include "..\peripherals\uart\uart.h"

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
	setvbuf(stdout, NULL, _IONBF, 0); // Wy��czenie buforowania  tekstu drukowanego przez printf

	StartUpReason();

	Init_RCC();
	Init_IO();
	Init_Peripherals();

	TimerLowSpeedStart(eTimerLowSpeed_Led, 10);			// Miganie LED6 co 1s
	TimerStart(eTimer_Setup, 100);						// Sprawdzanie
	TimerStart(eTimer_Potentiometer_Check, 50);
	TimerUStart(eTimerUS_Dac, 40);

	Init_Debug();
	Init_IWDG();

	Debug_Intro();

	while(1)
	{
		IWDG_Reset();						// Waruj - reset watchdoga
	    Led_Trig();
	    Debug_Handler();					// Obsluga konsoli debugowania
	    Potentiometer_Handler();			// Odczyt nastaw potencjometr�w
	}
	return 0;
}

