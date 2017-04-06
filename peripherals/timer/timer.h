#ifndef TIMER_H_
  #define TIMER_H_
/* Exported constants --------------------------------------------------------*/

  #define TIMER_OFF   -1
  #define TIMER_END   0
  #define TIMER_BUSY  1

/* Exported types ------------------------------------------------------------*/
enum eTimer		// Timer 1ms
{
	eTimer_LED,
	eTimer_Adc,
	eTimer_Key,
	eTimer_Test	,
	eTimer_Potentiometer_Check,
	eTimer_Servo_1,
	eTimer_Servo_2,
	eTimer_Servo_3,
	eTimer_Servo_4,
	eTimer_AppsTick,
	/*-----------------------------*/
	eNumberOfTimers
};

enum eTimerLowSpeed		// Timer 100ms
{
	eTimerLowSpeed_Test,
	eTimerLowSpeed_Led,
	eTimerLowSpeed_TimeDelayOn,
	/*-----------------------------*/
	eNumberOfLowSpeedTimers
};

enum eTimerFastSpeed		// Timer 100us
{
  eTimerUS_Adc,
/*-----------------------------*/
  eNumberOfUSTimers
};

/* Exported variable ---------------------------------------------------------*/
extern signed int Timer_us[eNumberOfUSTimers];        // 100us
extern signed int Timer_ms[eNumberOfTimers];          // 1ms
extern signed int Timer_ls[eNumberOfLowSpeedTimers];  // 100ms

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

/* Private function prototypes -----------------------------------------------*/

void TIM_Config(void);

void Init_Timer2(void);
void Init_PWMTimer(void);

#endif
