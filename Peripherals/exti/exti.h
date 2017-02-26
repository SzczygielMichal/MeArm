


#ifndef EXTI_H_
#define EXTI_H_

extern unsigned char CMX_FIFO_ReadIRQ;


void InitEXTI(void);

void Enable_EXTI(unsigned char On);

void EXTI1_IRQHandler(void);

void EXTI9_5_IRQHandler(void);


#endif
