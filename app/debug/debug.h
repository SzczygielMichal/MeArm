//---------------------------------------------------
//                   debug.h
//---------------------------------------------------


#ifndef DEBUG_H_
  #define DEBUG_H_


#define DEBUG_TX_BUFFER_MAX_COUNT   2000 //1152 - zmienione ze względu na wywietlanie słów kluczowych w konsoli
#define DEBUG_RX_BUFFER_MAX_COUNT   40


#define TextGreen     printf("\033[32m")
#define TextRed       printf("\033[31m")
#define TextWhite     printf("\033[37m")
#define TextYellow    printf("\033[33m")
#define TextBlue      printf("\033[34m")
#define TextMagenta   printf("\033[35m")
#define TextCyan      printf("\033[36m")
#define TextBlack     printf("\033[30m")
#define TextDefault   printf("\033[39m")

struct sScreenDownload
{
  unsigned char Screen[1024];
  unsigned char Ready;
  unsigned char Overtake;
  unsigned char ActLine;
};

extern struct sScreenDownload ScreenDownload;


void Init_Debug(void);

void UART_PutChar(int ch);
void Debug_UART_Rx_Handler(unsigned char Data);
void Debug_PutChar(unsigned char);
unsigned char Debug_SendData(unsigned char *Data, unsigned char DataCount);
void Debug_Handler(void);

void ScreenOvertake(void);
void ScreenOvertake_Handler(void *fPtrOnScreen);

#endif


