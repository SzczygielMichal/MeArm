//---------------------------------------------------
//                   debug.c
//---------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "stm32f10x.h"

//#include "setup\setup.h"
//#include "board\init.h"
//#include "peripherals\adc\adc.h"
#include "peripherals\uart\uart.h"
//#include "peripherals\timer\timer.h"
#include "debug.h"
//#include "board\board.h"
//#include "app\version\Version.h"

/*--- FUNKCJE PRYWATNE -------------------------------*/

unsigned char EmptyFunc(unsigned char	*string);
unsigned char HelloFunc(unsigned char	*string);
unsigned char ClsFunc(unsigned char		*string);
unsigned char ResetFunc(unsigned char	*string);
unsigned char PomocFunc(unsigned char	*string);
unsigned char InfoFunc(unsigned char	*string);
unsigned char BuildFunc(unsigned char	*string);
unsigned char LedOnFunc(unsigned char	*string);
unsigned char LedOffFunc(unsigned char	*string);
unsigned char RelOffFunc(unsigned char	*string);
unsigned char RelOnFunc(unsigned char	*string);
unsigned char ADCFunc(unsigned char		*string);
//unsigned char KeysFunc(unsigned char   *string);
//unsigned char KeyOnFunc(unsigned char  *string);
//unsigned char KeyOffFunc(unsigned char *string);
unsigned char MCU_IDFunc(unsigned char *string);
unsigned char CxxFunc(unsigned char *string);
unsigned char LoginToConsoleFunc(unsigned char *string);
unsigned char LoginToConsole_CheckLogin(unsigned char *string);
unsigned char LoginToConsole_CheckPassword(unsigned char *string);
unsigned char LogoutFromConsoleFunc(unsigned char *string);
unsigned short int McuRevRead(void);
unsigned short int McuDevRead(void);
unsigned char MCU_IDcode_Rev(unsigned char *string);
unsigned char MCU_IDcode_Dev(unsigned char *string);
unsigned char SetupPrint(unsigned char *string);

unsigned char SearchCommand(unsigned char *string);
void ConsoleClear(void);
void Console_Handler(void);

unsigned char Debug_SendData(unsigned char *Data, unsigned char DataCount);


/*----------------------------------------------------*/

// przejecie printf'a
void UART_PutChar(int ch)
{
	if(ch == '\n')
	{
		Debug_PutChar('\r');
	}
	Debug_PutChar(ch);
}


struct sDebug
{
  unsigned short TxCounter;
  unsigned short RxCounter;
  unsigned char TxBuffer[DEBUG_TX_BUFFER_MAX_COUNT];
  unsigned char RxBuffer[DEBUG_RX_BUFFER_MAX_COUNT];
};

struct sDebug Debug;

struct sProcDevice
{
  unsigned short Number;
  unsigned char Name[5];
};

#define PROC_DEV_COUNT  3

struct sProcDevice ProcesorDevice[PROC_DEV_COUNT] =
{
  {0x416, "Low "},
  {0x427, "Med+"},
  {0x436, "High"}
};

struct sProcRevision
{
  unsigned short Number;
  unsigned char Name[2];
};

#define PROC_REV_COUNT  3

struct sProcRevision ProcesorRevision[PROC_REV_COUNT] =
{
  {0x1000, "A"},
  {0x1008, "Z"},
  {0x1018, "Y"}
};

//--- Obsluga -----------------------------------------------------------------------------------------

#define CONSOLE_FUNCTION_QUANTITY 30

const struct sConsoleMenu
{
  struct
  {
    unsigned char KeyWord[20];
    unsigned char (*Function)(unsigned char *string);
    unsigned char Comment[80];
  }CF[CONSOLE_FUNCTION_QUANTITY];
} Console =
{
  {
    /*00*/ {"",			EmptyFunc,				""}, // pierwszy zostawic pusty !!!
    /*01*/ {"",			LoginToConsoleFunc,		"Logowanie do Konsoli"},
    /*02*/ {"clogout",	LogoutFromConsoleFunc,	"Wylogowanie z Konsoli"},
    /*03*/ {"?",		PomocFunc,				"Pomoc"},
    /*04*/ {"help",		PomocFunc,				"Pomoc"},
    /*05*/ {"hello",	HelloFunc,				"Testowo"},
    /*06*/ {"cls",		ClsFunc,				"Czyszczenie Ekranu"},
    /*07*/ {"reset",	ResetFunc,				"Reset Procesora"},
    /*08*/ {"idcode",	MCU_IDFunc,				"This ID ident. The MCU Partnu.and the die rev."},
    /*09*/ {"info",		InfoFunc,				"Opis Urządzenia"},
    /*10*/ {"build",	BuildFunc,				"Wersja Rewizji Oraz PCB"},
    /*11*/ {"ledon",	LedOnFunc,				"Zapalenie Diody LED"},
    /*12*/ {"ledoff",	LedOffFunc,				"Zgaszenie Diody LED"},
    /*13*/ {"rel1on",	RelOnFunc,				"Załączenie Przekaźnika 1 - Blokada Termiczna (KLH)"},
    /*14*/ {"rel1off",	RelOffFunc,				"Wyłączenie Przekaźnika 1 - Blokada Termiczna (KLH)"},
    /*15*/ {"rel2on",	RelOnFunc,				"Załączenie Przekaźnika 2 - Prąd Zwarciowy (I>>) - Bi"},
    /*16*/ {"rel2off",	RelOffFunc,				"Wyłączenie Przekaźnika 2 - Prąd Zwarciowy (I>>) - Bi"},
    /*17*/ {"rel3on",	RelOnFunc,				"Załączenie Przekaźnika 3 - Zanik Fazy - (P)"},
    /*18*/ {"rel3off",	RelOffFunc,				"Wyłączenie Przekaźnika 3 - Zanik Fazy - (P)"},
    /*19*/ {"rel4on",	RelOnFunc,				"Załączenie Przekaźnika 4 - Blokada Doziemienia (E-sp)"},
    /*20*/ {"rel4off",	RelOffFunc,				"Wyłączenie Przekaźnika 4 - Blokada Doziemienia (E-sp)"},
    /*21*/ {"rel5on",	RelOnFunc,				"Załączenie Przekaźnika 5 - Przełącznik Czasowy (T)"},
    /*22*/ {"rel5off",	RelOffFunc,				"Wyłączenie Przekaźnika 5 - Przełącznik Czasowy (T)"},
    /*23*/ {"rel6on",	RelOnFunc,				"Załączenie Przekaźnika 6 - Prąd Przeciążeniowy (I>)"},
    /*24*/ {"rel6off",	RelOffFunc,				"Wyłączenie Przekaźnika 6 - Prąd Przeciążeniowy (I>)"},
    /*25*/ {"rel7on",	RelOnFunc,				"Załączenie PRzekaźnika 7 - Blokada Doziemienia (E-sp) Bi"},
    /*26*/ {"rel7off",	RelOffFunc,				"Wyłączenie Przekaźnika 7 - Blokada Doziemienia (E-sp) Bi"},
    /*27*/ {"relon",	RelOnFunc,				"Załączenie Wszystkich Przekaźników"},
    /*28*/ {"reloff",	RelOffFunc,				"Wyłączenie Wszystkich Przekaźników"},
    /*29*/ {"setup",	SetupPrint,				"Wydruk ustawień"}

//    /*24*/ {"adc",     ADCFunc,         "odczyt danych analogowych z przetworników ADC"},
//    /*27*/ {"keys",    KeysFunc,        "Statusy kluczy"},
//    /*20*/ {"kon",     KeyOnFunc,       "Wł. (ON)  Kluczy: ZAS AUDIO, RSAG, ZAS RSAG"},
//    /*21*/ {"koff",    KeyOffFunc,      "Wył.(OFF) Kluczy: ZAS AUDIO, RSAG, ZAS RSAG"},
  }       
};

#define COMMAND_CODE_LOGIN_TO_CONSOLE 1
#define COMMAND_CODE_HELP 4 

struct sConsoleCtrl
{
  unsigned char CommandCode;
  unsigned char Step;
  unsigned char PasswordStars;
}ConsoleCtrl;

/// -------------------------------------------------------------------------------------------
///   Funkcje Konsoli
/// -------------------------------------------------------------------------------------------

unsigned char EmptyFunc(unsigned char *string)
{
  ConsoleClear();
  return F_RESULT_OK;
}

unsigned char CxxFunc(unsigned char *string)
{
  printf("\033[s");
  printf("\033[0;0f");
  printf("######\n\r######\n\r######\n\r"); 
  
  printf("\033[2;2f");
  printf("%4.2f",ADC_Control.ADC_1.Value[eADCi_DC_RP_SAG] * ADC_REAL_COEFF);
  
  printf("\033[u");
  ConsoleClear();
  return F_RESULT_OK;
}

unsigned char HelloFunc(unsigned char *string)
{
  printf("Witaj!!!! to ja Przekaźnik\n\r");
  ConsoleClear();
  return F_RESULT_OK;
}

unsigned char ClsFunc(unsigned char *string)
{
  printf("\033[2J");
  printf("\033[0;0f");
  ConsoleClear();
  return F_RESULT_OK;
}

unsigned char ResetFunc(unsigned char *string)
{
	ConsoleClear();
	/* Wylaczenie wszystkich przerwan */
	__asm volatile (" CPSID F       \n");
	NVIC_SystemReset();
	return F_RESULT_OK;
}

unsigned char PomocFunc(unsigned char *string)
{
  unsigned char i;

  TextYellow;
  printf("\n>>>\tSłowa Kluczowe\t<<<\n");
  TextGreen;
  for(i = 1; i < CONSOLE_FUNCTION_QUANTITY; i++)
  {
    if(i == COMMAND_CODE_LOGIN_TO_CONSOLE) continue;
    	printf("\t%s\t- %s\n\r", Console.CF[i].KeyWord, Console.CF[i].Comment);
  }
  TextDefault;
  ConsoleClear();
  return F_RESULT_OK;
}

unsigned char InfoFunc(unsigned char *string)
{
	printf("\033[2J");
	printf("\033[0;0f");
  
	TextYellow;
	printf("****************************************\n\r");
	printf("*        Przekaźnik elektroniczny      *\n\r");
	printf("*  Data kompilacji:    %14s  *\n\r", DATE_NOW);
	printf("*  Godzina kompilacji: %14s  *\n\r", TIME_NOW);
	printf("*  Data rewizji:       %14s  *\n\r", DATE_REV);
	printf("*  Godzina rewizji:    %14s  *\n\r", TIME_REV);
	printf("*  Rewizja:            %14d  *\n\r", REVISION);
	printf("*  Wersja Pcb:         %14s  *\n\r", BOARDREV);
	printf("****************************************\n\r");
  
	TextDefault;

	printf("---     Autor: Michał Szczygieł     ---\n\r");

	TextDefault;
	ConsoleClear();
	return F_RESULT_OK;
}

unsigned char BuildFunc(unsigned char *string)
{
  printf("\nRewizja:    %14d", REVISION);
  printf("\nWersja PCB: %14s\n", BOARDREV);
  ConsoleClear();
  return F_RESULT_OK;
}

/// -------------------------------------------------------------------------------------------
///   ADC
/// -------------------------------------------------------------------------------------------

unsigned char ADCFunc(unsigned char *string)
{
//  unsigned char i = 0;
//  unsigned char str[2][4] = {"OFF","ON "};
  
  printf("\033[2J");
  printf("\033[0;0f");

  printf("------ ADC CORE ------\n\r");
  printf("  Ucc    :\t%2.1f V\n\r", (ADC_Control.ADC_1.Value[eADCi_UZas1]   * ADC_REAL_COEFF)+0.2);
  printf("  Uwew   :\t%5.2f V\n\r",  ADC_Control.ADC_1.Value[eADCi_Uwew]    * ADC_REAL_COEFF);
  printf("  Iwew   :\t%5.2f mA\n\r",(ADC_Control.ADC_1.Value[eADCi_Iwew]    * ADC_CONST_FACTOR)/0.0114);
  printf("  Prąd   :\t%5.2f mA\n\r", ADC_Control.RealValue.Current);

  #ifdef DEBUG_ADC_C
    printf("------ ADC CONST ------\n\r"); 
    printf("  T  30st: %02X\n", *((uint8_t *)0x1FF800FA) ); 
    printf("  T 110st: %02X\n", *((uint8_t *)0x1FF800FB) ); 
    printf("  VREF   : %04X\n", *((uint16_t*)0x1FF800F8) );
  #endif
    
  ConsoleClear();
  return F_RESULT_OK;
}


/// -------------------------------------------------------------------------------------------
///   KEYS - przydadzą się przy kontroli dzielnika z wejcia pomiarowego przekładnika
/// -------------------------------------------------------------------------------------------
/*
unsigned char KeysFunc(unsigned char *string)
{
    unsigned short lines;
    printf("Statusy kluczy\n\r");
    

    KeysStatus();
    if(CheckLineS() == 0)      printf("%s: SIGNAL","Line S");    else printf("%s: ------","Line S");
   
    lines =  ( ~GPIOA->IDR );

    ConsoleClear();
    return F_RESULT_OK;
}
*/

/*
unsigned char KeyOnFunc(unsigned char *string)
{
  unsigned int temp;
 
  switch(ConsoleCtrl.Step)
  {
    case 0:
      printf("Keys ON:\ta - AUDIO\tr - RSAG\tz - WZM\n\rWybór: ");
      ConsoleCtrl.Step++;
      return F_RESULT_OK;
      break;
    case 1:
      if( strchr(string,'a') ){ ZAS_AUDIO_1;         temp++;}
      if( strchr(string,'r') ){ AC_DC_W_RSAG_0;      temp++;}
      if( strchr(string,'z') ){ ZAS_STAB_WZM_RSAG_0; temp++;}
      KeysStatus();
      if(!temp) return F_RESULT_ERR;
      ConsoleClear();
      return F_RESULT_OK;
      break;
  }
  return F_RESULT_ERR;
}
*/

/*
unsigned char KeyOffFunc(unsigned char *string)
{
  unsigned char temp;
 
  switch(ConsoleCtrl.Step)
  {
    case 0:
      printf("Keys OFF:\ta - AUDIO\tr - RSAG\tz - WZM\n\rWybór: ");
      ConsoleCtrl.Step++;
      return F_RESULT_OK;
      break;
    case 1:
      if( strchr(string,'a') ){ ZAS_AUDIO_0;         temp++;}
      if( strchr(string,'r') ){ AC_DC_W_RSAG_1;      temp++;}
      if( strchr(string,'z') ){ ZAS_STAB_WZM_RSAG_1; temp++;}
      KeysStatus();
            
      if(!temp) return F_RESULT_ERR;
      ConsoleClear();
      return F_RESULT_OK;
      break;
  }
  return F_RESULT_ERR;
}
*/


/// -------------------------------------------------------------------------------------------
///   MCU
/// -------------------------------------------------------------------------------------------

unsigned short int McuRevRead(void)
{
    return((unsigned short int)(DBGMCU->IDCODE >> 16) & 0xFFFF );
}

unsigned char MCU_IDcode_Rev(unsigned char *string)
{
  unsigned short rev;
  unsigned char i,j;
  
  rev = McuRevRead();

  for(i = 0; i < PROC_REV_COUNT; i++)
  {
    if(ProcesorRevision[i].Number == rev)
    {
      for(j = 0; j < sizeof(ProcesorRevision[i].Name); j++)
      {
        string[j] = ProcesorRevision[i].Name[j];
      }
      return F_RESULT_OK;
    }
  }
  return F_RESULT_ERR;
}

unsigned short int McuDevRead(void)
{
    return((unsigned short int)(DBGMCU->IDCODE)  & 0x0FFF );
}

unsigned char MCU_IDcode_Dev(unsigned char *string)
{
  unsigned short dev;
  unsigned char i,j;
  
  dev = McuDevRead();

  for(i = 0; i < PROC_DEV_COUNT; i++)
  {
    if(ProcesorDevice[i].Number == dev)
    {
      for(j = 0; j < sizeof(ProcesorDevice[i].Name); j++)
      {
        string[j] = ProcesorDevice[i].Name[j];
      }
      return F_RESULT_OK;
    }
  }
  return F_RESULT_ERR;
}


unsigned char MCU_IDFunc(unsigned char *string)
{
  unsigned char str[10];

  printf("Device Family : ");
  TextYellow;
  if(MCU_IDcode_Dev(str) == F_RESULT_OK)  printf("%s\n\r", str); else printf("Unknown\n\r");
  TextDefault;
  
  printf("Processor Rev : ");
  TextYellow;
  if(MCU_IDcode_Rev(str) == F_RESULT_OK)  printf("%s\n\r", str); else printf("Unknown\n\r");
  TextDefault;

  printf("Flash Size    : %04X\n", *((unsigned int *)0x1FF8004C) ); // Cat 1 and 2
  printf("Flash Size    : %04X\n", *((unsigned int *)0x1FF800CC) ); // Cat3,4, 5 and 6
  printf("Unique dev ID : %04X %04X %04X\n", *((unsigned int *)0x1FF800E4), *((unsigned int *)0x1FF800D4), *((unsigned int *)0x1FF800D0) );

  ConsoleClear();
  return F_RESULT_OK;
}

/// -------------------------------------------------------------------------------------------
///	sterowanie LED6
/// -------------------------------------------------------------------------------------------

unsigned char LedOnFunc(unsigned char *string)
{
	LED_ON;

	printf(":-->LED ON\n\n");
	ConsoleClear();
	return F_RESULT_OK;
}

unsigned char LedOffFunc(unsigned char *string)
{
	LED_OFF;
	printf(":-->LED OFF\n\n");
	ConsoleClear();
	return F_RESULT_OK;
}

/// -------------------------------------------------------------------------------------------
///	sterowanie Przeka�nikami
/// -------------------------------------------------------------------------------------------

unsigned char RelOnFunc(unsigned char *string)
{
	switch (ConsoleCtrl.CommandCode)
	{
		case 13:
			RELAY_1_ON;
			printf(":-->Przekaźnik %d ON\n\n", ConsoleCtrl.CommandCode-12); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 15:
			RELAY_2_ON;
			printf(":-->Przekaźnik %d ON\n\n", ConsoleCtrl.CommandCode-13); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 17:
			RELAY_3_ON;
			printf(":-->Przekaźnik %d ON\n\n", ConsoleCtrl.CommandCode-14); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 19:
			RELAY_4_ON;
			printf(":-->Przekaźnik %d ON\n\n", ConsoleCtrl.CommandCode-15); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 21:
			RELAY_5_ON;
			printf(":-->Przekaźnik %d ON\n\n", ConsoleCtrl.CommandCode-16); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 23:
			RELAY_6_ON;
			printf(":-->Przekaźnik %d ON\n\n", ConsoleCtrl.CommandCode-17); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 25:
			RELAY_7_ON;
			printf(":-->Przekaźnik %d ON\n\n", ConsoleCtrl.CommandCode-18); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 27:
			RELAY_1_ON;
			RELAY_2_ON;
			RELAY_3_ON;
			RELAY_4_ON;
			RELAY_5_ON;
			RELAY_6_ON;
			RELAY_7_ON;
			printf(":-->Przekaźniki Włączone\n\n");
			break;
		default:
			break;
	}
//	printf("%d", ConsoleCtrl.CommandCode);
	ConsoleClear();
	return F_RESULT_OK;
}

unsigned char RelOffFunc(unsigned char *string)
{
	switch (ConsoleCtrl.CommandCode)
	{
		case 14:
			RELAY_1_OFF;
			printf(":-->Przekaźnik %d OFF\n\n", ConsoleCtrl.CommandCode-13); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 16:
			RELAY_2_OFF;
			printf(":-->Przekaźnik %d OFF\n\n", ConsoleCtrl.CommandCode-14); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 18:
			RELAY_3_OFF;
			printf(":-->Przekaźnik %d OFF\n\n", ConsoleCtrl.CommandCode-15); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 20:
			RELAY_4_OFF;
			printf(":-->Przekaźnik %d OFF\n\n", ConsoleCtrl.CommandCode-16); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 22:
			RELAY_5_OFF;
			printf(":-->Przekaźnik %d OFF\n\n", ConsoleCtrl.CommandCode-17); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 24:
			RELAY_6_OFF;
			printf(":-->Przekaźnik %d OFF\n\n", ConsoleCtrl.CommandCode-18); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 26:
			RELAY_7_OFF;
			printf(":-->Przekaźnik %d OFF\n\n", ConsoleCtrl.CommandCode-19); // na pozycji 13 znajduje się funkcja włączenia przekaźnika
			break;
		case 28:
			RELAY_1_OFF;
			RELAY_2_OFF;
			RELAY_3_OFF;
			RELAY_4_OFF;
			RELAY_5_OFF;
			RELAY_6_OFF;
			RELAY_7_OFF;
			printf(":-->Przekaźniki Wyłączone\n\n");
			break;
		default:
			break;
	}
	ConsoleClear();
	return F_RESULT_OK;
}

unsigned char SetupPrint(unsigned char *string)
{
	unsigned int i_nom_c, i_nom_p, short_circuit_current_c, short_circuit_current_p;
	printf("\033[2J");
	printf("\033[0;0f");

	printf("\tDelay_Time_On:\t\t\t%d\n\r", pk_elS.Setup.TimeDelayOn);
	printf("\tShort_Circuit_Current_Mull:\t%d\n\r", pk_elS.Setup.Short_Circuit_Current);
	printf("\tNominal_Current_Pot:\t\t%d\n\r", pk_elS.Setup.Nominal_Current_Pot);
	if(pk_elS.Setup.Current_Transformer)
		printf("\tCurrent_Transformer:\t\t3mV/A\n\r");
	else
	{
		printf("\tCurrent_Transformer:\t\t1mV/A\n\r");
	}
	if(pk_elS.Setup.Rush_Delay)
		printf("\tRush_Delay:\t\t\tON\n\r");
	else
	{
		printf("\tRush_Delay:\t\t\tOFF\n\r");
	}
	if(pk_elS.Setup.Phase_Control)
		printf("\tPhase_Control:\t\t\tON\n\r");
	else
	{
		printf("\tPhase_Control:\t\t\tOFF\n\r");
	}
	if(pk_elS.Setup.A_Blockade)
		printf("\tA_Blockade:\t\t\tON\n\r");
	else
	{
		printf("\tA_Blockade:\t\t\tOFF\n\r");
	}
	if(pk_elS.Setup.PTC_Control)
		printf("\tPTC_Control_Time_Delay:\t\t500[ms] - ON\n\r");
	else
	{
		printf("\tPTC_Control_Time_Delay:\t\t100[ms] - OFF\n\r");
	}
	switch(pk_elS.Setup.Nominal_Current_Mull)
	{
		case Nominal_Current_Mull_TypeE_0_3:
			printf("\tNominal_Current_Mull:\t\tx0.3\n\r");
			break;
		case Nominal_Current_Mull_TypeE_1:
			printf("\tNominal_Current_Mull:\t\tx1\n\r");
			break;
		case Nominal_Current_Mull_TypeE_3:
			printf("\tNominal_Current_Mull:\t\tx3\n\r");
			break;
		case Nominal_Current_Mull_TypeE_10:
			printf("\tNominal_Current_Mull:\t\tx10\n\r");
			break;
		default:
			printf("\tNominal_Current_Mull:\t\tERR\n\r");
			break;
	}
	switch(pk_elS.Setup.Characteristic)
	{
		case Characteristic_TypeE_5s:
			printf("\tCharacteristic:\t\t\t5[s]\n\r");
			break;
		case Characteristic_TypeE_8s:
			printf("\tCharacteristic:\t\t\t8[s]\n\r");
			break;
		case Characteristic_TypeE_20s:
			printf("\tCharacteristic:\t\t\t20[s]\n\r");
			break;
		default:
			printf("\tCharacteristic:\t\t\tERR\n\r");
			break;
	}
	switch(pk_elS.Setup.Short_Circuit_Blockade)
	{
		case Short_Circuit_BlockadeE_No_Reset:
			printf("\tShort_Circuit_BlockadeE:\tNo_Reset\n\r");
			break;
		case Short_Circuit_BlockadeE_Electric_Reset:
			printf("\tShort_Circuit_BlockadeE:\tElectric_Reset\n\r");
			break;
		case Short_Circuit_BlockadeE_Mechanical_Reset:
			printf("\tShort_Circuit_BlockadeE:\tMechanical_Reset\n\r");
			break;
		case Short_Circuit_BlockadeE_Delay_Reset:
			printf("\tShort_Circuit_BlockadeE:\tDelay_Reset\n\r");
			break;
		default:
			printf("\tShort_Circuit_Blockade:\tERR\n\r");
			break;
	}

	i_nom_c = i_nom/10;
	i_nom_p = i_nom%10;
	short_circuit_current_c = short_circuit_current/10;
	short_circuit_current_p = short_circuit_current%10;
	printf("\tNominal_Current:\t\t%d.%d [A]\n\r",i_nom_c, i_nom_p);
	printf("\tShort_Circuit_Current:\t\t%d.%d [A]\n\r", short_circuit_current_c, short_circuit_current_p);

	ConsoleClear();
	return F_RESULT_OK;
}

/*

/// Funkcja inline na koncu za funkcja ja wywolujaca
inline void KeysStatus(void)
{
  if(ZAS_AUDIO )             printf("%s: ON  ", "AUDIO"); else printf("%s: OFF ", "AUDIO");
  if(AC_DC_W_RSAG == 0)      printf("%s: ON  ", "RSAG");  else printf("%s: OFF ", "RSAG");
  if(ZAS_STAB_WZM_RSAG == 0) printf("%s: ON  ", "WZM");   else printf("%s: OFF ", "WZM"); 
  if(AUDIO_SWITCH == 0)      printf("%s: CMX\t","SW");    else printf("%s: PWM\t","SW");
}
*/




/***********************************************************************************************
  #####   ###    ##   ####
  ##      ## #   ##   ##  ##
  ####    ##  #  ##   ##  ##
  ##      ##   # ##   ##  ##
  #####   ##    ###   ####    CONSOLE FUN
 ***********************************************************************************************/

unsigned char LogoutFromConsoleFunc(unsigned char *string)
{
  ConsoleCtrl.Step = 0;
  ConsoleCtrl.CommandCode = COMMAND_CODE_LOGIN_TO_CONSOLE;
  TextYellow;
  printf("Wylogowano z konsoli\n\r");
  TextDefault;
  return F_RESULT_OK;
}


/* Logowanie do konsoli */
unsigned char LoginToConsoleFunc(unsigned char *string)
{

  switch(ConsoleCtrl.Step)
  {
    case 0:
     printf("Podaj login: ");
      ConsoleCtrl.PasswordStars = 1;
      ConsoleCtrl.Step = 1;
      break;
    case 1:
      /* Sprawdzenie loginu */
      if(LoginToConsole_CheckLogin(string) == F_RESULT_OK)
     {
      ConsoleCtrl.Step = 2;
        printf("Podaj hasło: ");
      }
      else
      {
        TextRed;
        printf("Błędny login\n\r\n\r");
        TextDefault;
        printf("Podaj login : ");
        ConsoleCtrl.Step = 1;
      }
      break;
    case 2:
      /* Sprawdzenie hasła */
      if(LoginToConsole_CheckPassword(string) == F_RESULT_OK)
     {
        ConsoleCtrl.PasswordStars = 0;
        TextGreen;
        printf("Zalogowano\n\r\n\r");
       TextDefault;
        ConsoleClear();
      }
      else
      {
        TextRed;
        printf("Błędne hasło\n\r\n\r");
       TextDefault;
        printf("Podaj login: ");
        ConsoleCtrl.Step = 1;
      }
      break;
  }
  return F_RESULT_OK; /* Nie da się zmienić na inną funkcję bez podania prawidłowego loginu i hasła */
}

unsigned char LoginToConsole_CheckLogin(unsigned char *string)
{
  /* W przyszłości zrobić jakiś użytkowników */
  if(strcmp((const char*)string, "00") == 0)
    return F_RESULT_OK;
  else
    return F_RESULT_ERR;
}


unsigned char LoginToConsole_CheckPassword(unsigned char *string)
{
  /* W przyszłości zrobić jakiś użytkowników */
  if(strcmp((char*)string, "1457") == 0)
    return F_RESULT_OK;
  else
    return F_RESULT_ERR;
}

void Console_Handler(void)
{
  unsigned char str[20];
  unsigned char i,j;
  unsigned char temp;
  
  switch(Debug.RxBuffer[Debug.RxCounter-1])
  {
    case 'S': Debug.RxCounter = 0;  SetupPrint(NULL); break;
    
    default:
      if(Debug.RxBuffer[Debug.RxCounter-1] == 0x7F) // backspace
      {
        Debug.RxBuffer[--Debug.RxCounter] = 0;
        if(Debug.RxCounter > 0) 
        {
          Debug.RxBuffer[--Debug.RxCounter] = 0;
          Debug_PutChar(0x7F);
        }
        break;
      }

      if(Debug.RxBuffer[Debug.RxCounter-1] == '\r') // potwierdzenie enterem wchodzimy szukamy funkcji
      {
        Debug_PutChar('\r');
        Debug_PutChar('\n');
    
        if(Debug.RxCounter == 0xFF) return;
        j = 0;
        for(i = 0; i < Debug.RxCounter; i++)
        {
        	if((Debug.RxBuffer[i] == '\r')||(Debug.RxBuffer[i] == ' '))
        		break;
        	else
        		str[j++] = Debug.RxBuffer[i];
        }
        str[j] = 0;
        Debug.RxCounter = 0;
    
        switch(ConsoleCtrl.CommandCode)
        {
          case 0:
            temp = SearchCommand(str);
            if(temp != 0xff)
            {
              ConsoleCtrl.CommandCode = temp;
              if(Console.CF[ConsoleCtrl.CommandCode].Function(str) == F_RESULT_OK) return;
            }
            break;
          default :
            if(Console.CF[ConsoleCtrl.CommandCode].Function(str) == F_RESULT_OK) return;
            break;
        }
        printf("Nie rozpoznano komendy\n\r");
        ConsoleClear();
        break; /* Przyszedł enter nie wysyłamy innych znaczków */
      }
      
      if(ConsoleCtrl.PasswordStars == 0) 
      {
        Debug_PutChar(Debug.RxBuffer[Debug.RxCounter-1]); // odeślij znaczek
      }
      else
      {
        Debug_PutChar('*'); // wysyłanie gwiazdek - wprowadzanie hasła
      }
      break;
  }
}

unsigned char SearchCommand(unsigned char *string)
{
  unsigned char i;

  for(i = 0; i < CONSOLE_FUNCTION_QUANTITY; i++)
  {
    if( strcmp((const char*)string, (const char*)(Console.CF[i].KeyWord)) == 0) return i;
  }
  return 0xFF;
}
/***********************************************************************************************/
void Debug_PutChar(unsigned char Data)
{
	if(Debug.TxCounter < DEBUG_TX_BUFFER_MAX_COUNT)
	{
		Debug.TxBuffer[Debug.TxCounter++] = Data;
	}
}

unsigned char Debug_SendData(unsigned char *Data, unsigned char DataCount)
{
	unsigned char i;
	if((Debug.TxCounter + DataCount) < DEBUG_TX_BUFFER_MAX_COUNT)
	{
		for(i = 0; i < DataCount; i++) Debug.TxBuffer[Debug.TxCounter++] = *(Data++);
		return F_RESULT_OK;
	}
	return F_RESULT_ERR;
}

void Init_Debug(void)
{
//	ConsoleCtrl.CommandCode = COMMAND_CODE_LOGIN_TO_CONSOLE; // - wyłączyłem logowanie do konsoli - należy tą opcję włączyć
	ConsoleCtrl.CommandCode = 0;
	ConsoleCtrl.Step = 0;
}

void Debug_Handler(void)
{
	if(Debug.TxCounter > 0)
	{
		if(sUART1.TxCounter == sUART1.TxCount) // zakończono nadawanie, rozpoczynamy nowe
		{
			SendUART1(Debug.TxBuffer, Debug.TxCounter);
			Debug.TxCounter = 0;
		}
	}
}

void Debug_UART_Rx_Handler(unsigned char Data)
{
	if(Debug.RxCounter < DEBUG_RX_BUFFER_MAX_COUNT)
	{
		Debug.RxBuffer[Debug.RxCounter++] = Data;
		Console_Handler();
	}
	else
		Debug.RxCounter = 0;
}

void ConsoleClear(void)
{
	ConsoleCtrl.CommandCode = 0;
	ConsoleCtrl.Step = 0;
}
/***********************************************************************************************/


