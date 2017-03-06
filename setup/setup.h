/*!
******************************************************************************
\file setup.c
\date 09-03-2015
\author Szczygie�, Micha�
\brief Ustawienie wartosci poczatkowych

******************************************************************************
*/

#ifndef SETUP_H_
  #define SETUP_H_

#include "main.h"
#include "..\board\board.h"       // Date 04.12.2014

struct _setupS
{
	unsigned int Nominal_Current_Pot;				// wynik z ADC1[3] - Potencjometr POT3 - nastawa pr�du nominalnego 7 - 25A
	unsigned int TimeDelayOn;						// wynik z ADC1[6] - Potencjiometr POT1 - czas op�nienia w��czenia przeka�nika REL5
	unsigned int Short_Circuit_Current;				// wynik z ADC1[5] - Potencjiometr POT2 - pr�d zwarciowy wynosi 3-12x Nominal_Current
	unsigned int Nominal_Current;					// pr�d nominalny = Nominal_Current_Pot*Nominal_Current_Mull

	unsigned short Crc;
} __attribute__((packed));


struct _sMeArm
{
	struct _setupS Setup;
	struct _statusS statusS;
};

extern struct _sMeArm sMeArm;

#endif

