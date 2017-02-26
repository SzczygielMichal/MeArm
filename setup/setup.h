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
#include "board\board.h"       // Date 04.12.2014
#include "dbg_ctrl.h"

enum _Current_Transformer_TypeE
{
	Current_Transformer_TypeE_1mV_A,
	Current_Transformer_TypeE_3mV_A
};

enum _Nominal_Current_Mull_TypeE
{
	Nominal_Current_Mull_TypeE_0_3,
	Nominal_Current_Mull_TypeE_1,
	Nominal_Current_Mull_TypeE_3,
	Nominal_Current_Mull_TypeE_10
};

enum _Rush_DelayE
{
	Rush_DelayE_Off,
	Rush_DelayE_On
};

enum _Phase_ControlE
{
	Phase_ControlE_Off,
	Phase_ControlE_On
};

enum _A_BlockadeE
{
	A_BlockadeE_Off,
	A_BlockadeE_On
};

enum _Characteristic_TypeE
{
	Characteristic_TypeE_5s,
	Characteristic_TypeE_8s,
	Characteristic_TypeE_20s
};

enum _Short_Circuit_BlockadeE	// blokada doziemienia
{
	Short_Circuit_BlockadeE_No_Reset,
	Short_Circuit_BlockadeE_Electric_Reset,
	Short_Circuit_BlockadeE_Mechanical_Reset,
	Short_Circuit_BlockadeE_Delay_Reset
};

enum _PTC_ControlE
{
	PTC_ControlE_Off,
	PTC_ControlE_On
};

struct _setupS
{
	unsigned int Nominal_Current_Pot;				// wynik z ADC1[3] - Potencjometr POT3 - nastawa pr�du nominalnego 7 - 25A
	unsigned int TimeDelayOn;						// wynik z ADC1[6] - Potencjiometr POT1 - czas op�nienia w��czenia przeka�nika REL5
	unsigned int Short_Circuit_Current;				// wynik z ADC1[5] - Potencjiometr POT2 - pr�d zwarciowy wynosi 3-12x Nominal_Current

	unsigned int Nominal_Current;					// pr�d nominalny = Nominal_Current_Pot*Nominal_Current_Mull

	unsigned int Current_Transformer:		1;		// typ przek�adnika 0 = 3mV/A (domyslnie), 1 = 1mV/A
	unsigned int Nominal_Current_Mull:		2;		// mno�nik pr�du znamionowego
	unsigned int Rush_Delay:				1;		// op�nienie kontroli pr�du rozruchu
	unsigned int Phase_Control:				1;		// kontrola zaniku fazy 1 - kontrola zaniku fazy
	unsigned int A_Blockade:				1;		// a-blokada
	unsigned int Characteristic:			2;		// charkterystyka zachowania si� a-wyzwolenia
	unsigned int Short_Circuit_Blockade:	2;		// blokada doziemienia
	unsigned int PTC_Control:				1;		// czas op�nienia zadzia�ania PTC

	unsigned short Crc;
} __attribute__((packed));


struct _pk_elS
{
	struct _setupS Setup;
	struct _statusS statusS;
};

extern struct _pk_elS pk_elS;

#endif

