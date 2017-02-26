//---------------------------------------------------
//                   Init.h
//---------------------------------------------------/**
#ifndef _INIT_H_
	#define _INIT_H_

	#define bool _Bool
	#define FALSE 0
	#define TRUE !FALSE

	extern const unsigned int nominalCcurrentTable[19][2];
	extern const unsigned int shortCircuitCurrentTable[10][2];
	extern const unsigned int delayTimeOnTable[11][2];
	extern unsigned int i_nom, mull;
	extern unsigned int short_circuit_current;

	void Init_RCC(void);
	void Init_IO(void);
	void Init_Peripherals(void);
	void StartUpReason(void);

	void Init_Setup(void);
	unsigned char pot(unsigned int **tab, unsigned int pom);

#endif //clk.h    

