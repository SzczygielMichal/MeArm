//---------------------------------------------------
//                   Init.h
//---------------------------------------------------/**
#ifndef _INIT_H_
	#define _INIT_H_

	#define bool _Bool
	#define FALSE 0

	#define TRUE !FALSE

	extern unsigned int i_nom, mull;

	void Init_RCC(void);
	void Init_IO(void);
	void Init_Peripherals(void);
	void StartUpReason(void);

	unsigned char pot(unsigned int **tab, unsigned int pom);

#endif

