

#ifndef TIMER_H_
  #define TIMER_H_


/* Exported constants --------------------------------------------------------*/


  #define TIMER_OFF   -1
  #define TIMER_END   0
  #define TIMER_BUSY  1



#define DELAY_CONNECTION_AFTER_R_READY  100



/* Exported types ------------------------------------------------------------*/
enum eTimer
{
	eTimer_Test,
	eTimer_PTC_Check,
	eTimer_PTC_Delay,
	eTimer_TimeDelayOnCheck,
	eTimer_ShortCircuit_Check,
	eTimer_Setup,
	eTimer_Potentiometer_Check,
	eTimer_Adc,
	eTimer_InterruptConnection,
	eTimer_AppsTick,
	/*-----------------------------*/
	eNumberOfTimers
};

struct sFrequency_Counter
{
  unsigned int ChannelPositive;
  unsigned int ChannelNegative;
  unsigned char PositiveOverflow;
  unsigned char NegativeOverflow;
};


enum eTimerLowSpeed
{
	eTimerLowSpeed_StartUp,
	eTimerLowSpeed_Test,
	eTimerLowSpeed_Led,
	eTimerLowSpeed_Reset,
	eTimerLowSpeed_TimeDelayOn,

	/*-----------------------------*/
	eNumberOfLowSpeedTimers
};

enum eTimerFastSpeed
{
  eTimerUS_Adc,
  eTimerUS_Dac,

/*-----------------------------*/
  eNumberOfUSTimers,
};


/* Exported variable ---------------------------------------------------------*/
extern signed int Timer_us[eNumberOfUSTimers];        // 100us
extern signed int Timer_ms[eNumberOfTimers];          // 1ms
extern signed int Timer_ls[eNumberOfLowSpeedTimers];  // 100ms

extern unsigned int TimerCS;
extern struct sFrequency_Counter Frequency_Counter;


/* Exported functions ------------------------------------------------------- */



/* Exported functions ------------------------------------------------------- */
#define TimerUStart(TimerNr, Time)     Timer_us[TimerNr] = Time
#define TimerUStop(TimerNr)            Timer_us[TimerNr] = TIMER_OFF
#define TimerUEnded(TimerNr)           Timer_us[TimerNr] = TIMER_END
#define TimerUStatus(TimerNr)          Timer_us[TimerNr]
#define TimerURead(TimerNr)            Timer_us[TimerNr]


#define TimerStart(TimerNr, Time)     Timer_ms[TimerNr] = Time
#define TimerStop(TimerNr)            Timer_ms[TimerNr] = TIMER_OFF
#define TimerEnded(TimerNr)           Timer_ms[TimerNr] = TIMER_END
#define TimerStatus(TimerNr)          Timer_ms[TimerNr]
#define TimerRead(TimerNr)            Timer_ms[TimerNr]


#define TimerLowSpeedStart(TimerNr, Time)   Timer_ls[TimerNr] = Time
#define TimerLowSpeedStop(TimerNr)          Timer_ls[TimerNr] = TIMER_OFF
#define TimerLowSpeedEnded(TimerNr)         Timer_ls[TimerNr] = TIMER_END
#define TimerLowSpeedStatus(TimerNr)        Timer_ls[TimerNr]
#define TimerLowSpeedRead(TimerNr)          Timer_ls[TimerNr]


#define TimerCSignalStart() TimerCS = 0
#define TimerCSignalStop()  TimerCS = 0xFFFFFFFF
#define TimerCSignalRead()  TimerCS


enum eEltelnetTimerType
{
  eEltelnetTimerType_Response, //  44us
  eEltelnetTimerType_Synchro,  // 249us
};

/* Private function prototypes -----------------------------------------------*/

void TIM_Config(void);

void Init_Timer(void);
void Init_LowSpeedTimer(void);
void Init_FastSpeedTimer(void);

#endif

