
#ifndef _BOARD_H
  #define _BOARD_H

/**
 * \brief Platforma sprzêtowa - uzale¿niona od wyboru pliku nag³ówkowego.
 */
  #include "..\board\board_v_26_02_2017.h"

  #define PRODUCENT_INFO  "Micha³ Szczygie³ - Ruda sl¹ska"
  #define PRODUCENT_INFO_SHORT  "Micha³ Szczygie³"
  
  #define ON  1
  #define OFF 0

/* MACROs for SET, RESET or TOGGLE Output port */
  #define GPIO_HIGH(a,b) 	a->BSRRL = b
  #define GPIO_LOW(a,b)		a->BSRRH = b
  #define GPIO_TOGGLE(a,b) 	a->ODR ^= b
   
  #define SET_PIN(port, pin)    port->BSRRL = ((uint16_t) GPIO_BSRR_BS_##pin)
  #define RESET_PIN(port, pin)  port->BSRRH = ((uint16_t)((GPIO_BSRR_BR_##pin) >> 16))
  #define CLEAR_PIN(port, pin)  port->BSRRH = ((uint16_t)((GPIO_BSRR_BR_##pin) >> 16))
  #define STATE_PIN(port, pin)  (port->IDR &  ((uint16_t) GPIO_OTYPER_IDR_##pin))

  typedef enum 
  {
    LED1 = 0,
  } Led_TypeDef;

  typedef enum 
  {
    COM1 = 0
  } COM_TypeDef;

  extern unsigned int Build;

  unsigned int ConvertIntEndian(unsigned int Data);
  void RewriterCharBuffer(unsigned char *Dst, unsigned char *Src, unsigned short Count);

  void Reset_TimeStart(void); 
  void Reset_Handler(void);

#endif
