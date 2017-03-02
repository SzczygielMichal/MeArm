/**
 ******************************************************************************
 * @file       Main.h
 * @brief      134 Przekaźnik Elektroniczny
 * @brief      ELTEL Sp. z o.o. Katowice 40-312 Milowicka 1E
 * @authors    Micha� Szczygie�
 * @date       17.12.2014
 * @version    PCB 04.12.2014
 * @details    Processor: STM32L151RD,
 *             Q = 4MHz,
 *             SYS_CLK, AHB, APB1, APB2 - Taktowanie w pliku init.c
 ******************************************************************************
 * <h2><center>&copy; COPYRIGHT 2014 ELTEL</center></h2>
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H_
 #define __MAIN_H_

void Debug_Intro (void);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
 #define F_RESULT_OK   1
 #define F_RESULT_ERR  0

 #define F_ENABLE    1
 #define F_DISABLE   0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

struct sSerialNr
{
 unsigned int Part[3];
}__attribute__((packed));

struct _statusS
{
 struct _startUpS
 {
   union
   {
     unsigned char Entire;
     struct
     {
       unsigned char LowPowerReset :1;
       unsigned char IwdgReset     :1;
       unsigned char SoftReset     :1;
       unsigned char PdrReset      :1;
       unsigned char PinReset      :1;
     };
   };
 }__attribute__((packed)) startUpS;
};

void Debug_Intro (void);
extern char op;

#endif /* MAIN_H_ */
