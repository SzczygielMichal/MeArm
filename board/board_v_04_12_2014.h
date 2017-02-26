 /**
  * @brief   Przeka�nik elektroniczny PCB 17.12.2014
  * @author  Micha� Szczygie�
  * @version V1.0
  * @date    04.12.2014
  * @brief   Definicje wejs� i wyjs�
  * <h2><center>&copy; COPYRIGHT 2014 ELTEL</center></h2>
  */

#ifndef PK_EL_BOARD_V_04_12_2014_H
	#define PK_EL_BOARD_V_04_12_2014_H

#define BOARDREV	"V_04_12_2014"

#include "board.h"


/// Odczyt bitow 0 do 7 portu C
#define DIPSWITCH_1   (( ~GPIOC->IDR ) &  0x00ff)

/// Odczyt bitow 8 do 10 portu C
#define DIPSWITCH_2   ((( ~GPIOC->IDR ) >> 8) & 0x0007)

/// Odczyt bitu 11 portu C
#define DIPSWITCH_3   ((( ~GPIOC->IDR ) >> 11) & 0x0001)

/*-----------------------------------------------------------------*/

/** @addtogroup LED6
  * @{
  */
#define LEDn					6
#define LED6_PIN				((uint16_t)0x1000)  /*!< Pin 13 selected portu C*/
#define LED6_GPIO_PORT		GPIOC
#define LED_ON				GPIOC->ODR |=  (1<<13)
#define LED_OFF				GPIOC->ODR &= ~(1<<13)
/**
  * @}
  */

/** @addtogroup TIME
  * @{
  */
#define TIMEn					11
#define TIME_PIN				((uint16_t)0x0800)  /*!< Pin 11 selected portu B*/
#define TIME_GPIO_PORT		GPIOB
#define TIME_ON				GPIOB->ODR |=  (1<<11)
#define TIME_OFF				GPIOB->ODR &= ~(1<<11)
/**
  * @}
  */

/// Sterowanie przekaźnikiem elektronicznym na wejsciu toru pomiaru prądu obciążenia silnika

#define CURRENT_COM_NO    WRITE_REG(GPIOA->BSRRL, (uint16_t)(GPIO_BSRR_BS_8))       // SET_bit lo_reg
#define CURRENT_COM_NC    WRITE_REG(GPIOA->BSRRH, (uint16_t)(GPIO_BSRR_BR_8 >> 16)) // RST_bit hi_reg

/// Sterowanie przekaźnikami

#define RELAY_1_ON		WRITE_REG(GPIOB->BSRRL, (uint16_t)(GPIO_BSRR_BS_6))       // SET_bit lo_reg
#define RELAY_1_OFF		WRITE_REG(GPIOB->BSRRH, (uint16_t)(GPIO_BSRR_BR_6 >> 16)) // RST_bit hi_reg

#define RELAY_2_ON		WRITE_REG(GPIOB->BSRRL, (uint16_t)(GPIO_BSRR_BS_8))       // SET_bit lo_reg
#define RELAY_2_OFF		WRITE_REG(GPIOB->BSRRH, (uint16_t)(GPIO_BSRR_BR_8 >> 16)) // RST_bit hi_reg

#define RELAY_3_ON		WRITE_REG(GPIOC->BSRRL, (uint16_t)(GPIO_BSRR_BS_14))       // SET_bit lo_reg
#define RELAY_3_OFF		WRITE_REG(GPIOC->BSRRH, (uint16_t)(GPIO_BSRR_BR_14 >> 16)) // RST_bit hi_reg

#define RELAY_4_ON		WRITE_REG(GPIOB->BSRRL, (uint16_t)(GPIO_BSRR_BS_7))       // SET_bit lo_reg
#define RELAY_4_OFF		WRITE_REG(GPIOB->BSRRH, (uint16_t)(GPIO_BSRR_BR_7 >> 16)) // RST_bit hi_reg

#define RELAY_5_ON		WRITE_REG(GPIOC->BSRRL, (uint16_t)(GPIO_BSRR_BS_15))       // SET_bit lo_reg
#define RELAY_5_OFF		WRITE_REG(GPIOC->BSRRH, (uint16_t)(GPIO_BSRR_BR_15 >> 16)) // RST_bit hi_reg

#define RELAY_6_ON		WRITE_REG(GPIOB->BSRRL, (uint16_t)(GPIO_BSRR_BS_5))       // SET_bit lo_reg
#define RELAY_6_OFF		WRITE_REG(GPIOB->BSRRH, (uint16_t)(GPIO_BSRR_BR_5 >> 16)) // RST_bit hi_reg

#define RELAY_7_ON		WRITE_REG(GPIOB->BSRRL, (uint16_t)(GPIO_BSRR_BS_9))       // SET_bit lo_reg
#define RELAY_7_OFF		WRITE_REG(GPIOB->BSRRH, (uint16_t)(GPIO_BSRR_BR_9 >> 16)) // RST_bit hi_reg

/*-----------------------------------------------------------------*/

// Sygnały wejsciowe

#define CHECK_TIME_LINE				( ( ( GPIOA->IDR ) &  (1<<7)) >> 7  )	// Wejscie linii wyzwalającej odmierzanie czasu
#define CHECK_PTC_LINE				( ( ( GPIOD->IDR ) &  (1<<2)) >> 2  )	// Wejscie linii kontrolującej temperaturę silnika poprzez PTC
#define CHECK_SHORT_CIRCUIT_LINE	( ( ( GPIOC->IDR ) &  (1<<12)) >> 12  )	// Wejscie linii kontrolującej doziemienie
#define CHECK_CRAMP					( ( ( GPIOB->IDR ) &  (1<<10)) >> 10  )	// kontrola typu podłączonego przekładnika

#define SET_2_SEC_OUTPUT			MODIFY_REG(GPIOA->MODER,	GPIO_MODER_MODER2,	GPIO_MODER_MODER2_0); MODIFY_REG(GPIOA->PUPDR,    GPIO_PUPDR_PUPDR2,    GPIO_PUPDR_PUPDR2_0) // 01: Pull-UP

#define SET_2_SEC_INPUT				MODIFY_REG(GPIOA->MODER,	GPIO_MODER_MODER2,	0); MODIFY_REG(GPIOA->PUPDR,    GPIO_PUPDR_PUPDR2, 0)
#define CHECK_2_SEC					( ( ( GPIOA->IDR ) &  (1<<2)) >> 2  )	// sprawdzenie czy nastąpił reset z opóźnieniem

#endif	//PK_EL_BOARD_V_04_12_2014_H
