 /**
  * @brief   MeArm PCB 26.02.2017
  * @author  Micha³ Szczygie³
  * @version V1.0
  * @date    26.02.2017
  * @brief   Definicje wejœ i wyjœ
  * <h2><center>&copy; COPYRIGHT 2014 Szczygie³ </center></h2>
  */

#ifndef MEARM_BOARD_V_26_02_2017_H
	#define MEARM_BOARD_V_26_02_2017_H

#define BOARDREV	"V_26_02_2017"

#include "board.h"


/** @addtogroup LED1
  * @{
  */
#define LEDn				1
#define LED1_PIN			((uint16_t)0x1000)  /*!< Pin 13 selected port C*/
#define LED1_GPIO_PORT		GPIOC
#define LED_ON				GPIOC->ODR &= ~(1<<13)
#define LED_OFF				GPIOC->ODR |= (1<<13)
/**
  * @}
  */

/** @addtogroup TIME
  * @{
  */
//#define TIMEn				11
//#define TIME_PIN			((uint16_t)0x0800)  /*!< Pin 11 selected port B*/
//#define TIME_GPIO_PORT	GPIOB
//#define TIME_ON			GPIOB->ODR |=  (1<<11)
//#define TIME_OFF			GPIOB->ODR &= ~(1<<11)
/**
  * @}
  */

//
//	Pozostawione dla przyk³adu gdybym chcia³ sterowac wyjœciami binarnymi
//

#define CURRENT_COM_NO    WRITE_REG(GPIOA->BSRRL, (uint16_t)(GPIO_BSRR_BS_8))       // SET_bit lo_reg
#define CURRENT_COM_NC    WRITE_REG(GPIOA->BSRRH, (uint16_t)(GPIO_BSRR_BR_8 >> 16)) // RST_bit hi_reg

#endif	//BOARD_V_26_02_2016_H
