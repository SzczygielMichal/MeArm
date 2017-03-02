//---------------------------------------------------
//                   signal.c
//---------------------------------------------------

#include "stm32f10x.h"
//#include "STM32F407IG.h"
#include "..\board\board.h"
#include "signal.h"
#include "..\peripherals\timer\timers.h"
//#include "..\app\events\events.h"
#include "..\peripherals\adc\adc.h"


struct sSignalGen SignalGen;


#define MAX_NR_SIGNAL_GEN 7 // najwyższy numer generowanego sygnału

/* Podtrzymywanie zewnętrznego KSR */
#define EXT_KSR_HOLD_TIME 70
#define EXT_KSR_DELAYCHECK_TIME 20

