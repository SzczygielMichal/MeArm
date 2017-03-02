//---------------------------------------------------
//                   debug.c
//---------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "stm32f10x.h"

#include "setup\setup.h"
#include "board\init.h"
#include "peripherals\adc\adc.h"
#include "peripherals\uart\uart.h"
#include "peripherals\timer\timer.h"
#include "debug.h"
#include "board\board.h"
//#include "app\version\Version.h"

/*--- FUNKCJE PRYWATNE -------------------------------*/

unsigned char EmptyFunc(unsigned char	*string);
unsigned char HelloFunc(unsigned char	*string);
unsigned char ClsFunc(unsigned char		*string);
unsigned char ResetFunc(unsigned char	*string);
unsigned char PomocFunc(unsigned char	*string);
unsigned char LedOnFunc(unsigned char	*string);
unsigned char LedOffFunc(unsigned char	*string);
unsigned char LoginToConsoleFunc(unsigned char *string);
unsigned char LoginToConsole_CheckLogin(unsigned char *string);
unsigned char LoginToConsole_CheckPassword(unsigned char *string);
unsigned char LogoutFromConsoleFunc(unsigned char *string);
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
    /*07*/ {"ledon",	LedOnFunc,				"Zapalenie Diody LED"},
    /*08*/ {"ledoff",	LedOffFunc,				"Zgaszenie Diody LED"},
    /*09*/ {"setup",	SetupPrint,				"Wydruk ustawień"}

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


