#include "stm32l1xx.h"
#include "peripherals\dac\dac.h"
#include "peripherals\timer\timer.h"
#include "setup\setup.h"
#include "cmsis_lib\include\stm32l1xx_dac.h"
#include "cmsis_lib\include\stm32l1xx_rcc.h"
#include "cmsis_lib\include\misc.h"

const int  sinewave[256]=
{
	0x0800, 0x0832, 0x0864, 0x0896, 0x08C8, 0x08FA, 0x092C, 0x095E, 0x098F, 0x09C0, 0x09F1, 0x0A22, 0x0A52, 0x0A82, 0x0AB1, 0x0AE1,
	0x0B0F, 0x0B3D, 0x0B6B, 0x0B98, 0x0BC5, 0x0BF1, 0x0C1C, 0x0C47, 0x0C71, 0x0C9B, 0x0CC3, 0x0CEB, 0x0D13, 0x0D39, 0x0D5F, 0x0D84,
	0x0DA8, 0x0DCB, 0x0DED, 0x0E0E, 0x0E2F, 0x0E4E, 0x0E6C, 0x0E8A, 0x0EA6, 0x0EC2, 0x0EDC, 0x0EF5, 0x0F0E, 0x0F25, 0x0F3B, 0x0F50,
	0x0F64, 0x0F76, 0x0F88, 0x0F98, 0x0FA7, 0x0FB5, 0x0FC2, 0x0FCE, 0x0FD8, 0x0FE1, 0x0FE9, 0x0FF0, 0x0FF6, 0x0FFA, 0x0FFD, 0x0FFF,
	0x0fff, 0x0FFF, 0x0FFD, 0x0FFA, 0x0FF6, 0x0FF0, 0x0FE9, 0x0FE1, 0x0FD8, 0x0FCE, 0x0FC2, 0x0FB5, 0x0FA7, 0x0F98, 0x0F88, 0x0F76,
	0x0F64, 0x0F50, 0x0F3B, 0x0F25, 0x0F0E, 0x0EF5, 0x0EDC, 0x0EC2, 0x0EA6, 0x0E8A, 0x0E6C, 0x0E4E, 0x0E2F, 0x0E0E, 0x0DED, 0x0DCB,
	0x0DA8, 0x0D84, 0x0D5F, 0x0D39, 0x0D13, 0x0CEB, 0x0CC3, 0x0C9B, 0x0C71, 0x0C47, 0x0C1C, 0x0BF1, 0x0BC5, 0x0B98, 0x0B6B, 0x0B3D,
	0x0B0F, 0x0AE1, 0x0AB1, 0x0A82, 0x0A52, 0x0A22, 0x09F1, 0x09C0, 0x098F, 0x095E, 0x092C, 0x08FA, 0x08C8, 0x0896, 0x0864, 0x0832,
	0x0800, 0x07CD, 0x079B, 0x0769, 0x0737, 0x0705, 0x06D3, 0x06A1, 0x0670, 0x063F, 0x060E, 0x05DD, 0x05AD, 0x057D, 0x054E, 0x051E,
	0x04F0, 0x04C2, 0x0494, 0x0467, 0x043A, 0x040E, 0x03E3, 0x03B8, 0x038E, 0x0364, 0x033C, 0x0314, 0x02EC, 0x02C6, 0x02A0, 0x027B,
	0x0257, 0x0234, 0x0212, 0x01F1, 0x01D0, 0x01B1, 0x0193, 0x0175, 0x0159, 0x013D, 0x0123, 0x010A, 0x00F1, 0x00DA, 0x00C4, 0x00AF,
	0x009B, 0x0089, 0x0077, 0x0067, 0x0058, 0x004A, 0x003D, 0x0031, 0x0027, 0x001E, 0x0016, 0x000F, 0x0009, 0x0005, 0x0002, 0x0000,
	0x0000, 0x0000, 0x0002, 0x0005, 0x0009, 0x000F, 0x0016, 0x001E, 0x0027, 0x0031, 0x003D, 0x004A, 0x0058, 0x0067, 0x0077, 0x0089,
	0x009B, 0x00AF, 0x00C4, 0x00DA, 0x00F1, 0x010A, 0x0123, 0x013D, 0x0159, 0x0175, 0x0193, 0x01B1, 0x01D0, 0x01F1, 0x0212, 0x0234,
	0x0257, 0x027B, 0x02A0, 0x02C6, 0x02EC, 0x0314, 0x033C, 0x0364, 0x038E, 0x03B8, 0x03E3, 0x040E, 0x043A, 0x0467, 0x0494, 0x04C2,
	0x04F0, 0x051E, 0x054E, 0x057D, 0x05AD, 0x05DD, 0x060E, 0x063F, 0x0670, 0x06A1, 0x06D3, 0x0705, 0x0737, 0x0769, 0x079B, 0x07CD
};
const int sawtoothwave[256]= //sawtooth wave
{
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
	0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
	0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
	0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
	0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
	0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
	0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff
};
const int rewsawtoothwave[256]= //reverse sawtooth wave
{
	0xff,0xfe,0xfd,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,0xf6,0xf5,0xf4,0xf3,0xf2,0xf1,0xf0,
	0xef,0xee,0xed,0xec,0xeb,0xea,0xe9,0xe8,0xe7,0xe6,0xe5,0xe4,0xe3,0xe2,0xe1,0xe0,
	0xdf,0xde,0xdd,0xdc,0xdb,0xda,0xd9,0xd8,0xd7,0xd6,0xd5,0xd4,0xd3,0xd2,0xd1,0xd0,
	0xcf,0xce,0xcd,0xcc,0xcb,0xca,0xc9,0xc8,0xc7,0xc6,0xc5,0xc4,0xc3,0xc2,0xc1,0xc0,
	0xbf,0xbe,0xbd,0xbc,0xbb,0xba,0xb9,0xb8,0xb7,0xb6,0xb5,0xb4,0xb3,0xb2,0xb1,0xb0,
	0xaf,0xae,0xad,0xac,0xab,0xaa,0xa9,0xa8,0xa7,0xa6,0xa5,0xa4,0xa3,0xa2,0xa1,0xa0,
	0x9f,0x9e,0x9d,0x9c,0x9b,0x9a,0x99,0x98,0x97,0x96,0x95,0x94,0x93,0x92,0x91,0x90,
	0x8f,0x8e,0x8d,0x8c,0x8b,0x8a,0x89,0x88,0x87,0x86,0x85,0x84,0x83,0x82,0x81,0x80,
	0x7f,0x7e,0x7d,0x7c,0x7b,0x7a,0x79,0x78,0x77,0x76,0x75,0x74,0x73,0x72,0x71,0x70,
	0x6f,0x6e,0x6d,0x6c,0x6b,0x6a,0x69,0x68,0x67,0x66,0x65,0x64,0x63,0x62,0x61,0x60,
	0x5f,0x5e,0x5d,0x5c,0x5b,0x5a,0x59,0x58,0x57,0x56,0x55,0x54,0x53,0x52,0x51,0x50,
	0x4f,0x4e,0x4d,0x4c,0x4b,0x4a,0x49,0x48,0x47,0x46,0x45,0x44,0x43,0x42,0x41,0x40,
	0x3f,0x3e,0x3d,0x3c,0x3b,0x3a,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30,
	0x2f,0x2e,0x2d,0x2c,0x2b,0x2a,0x29,0x28,0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20,
	0x1f,0x1e,0x1d,0x1c,0x1b,0x1a,0x19,0x18,0x17,0x16,0x15,0x14,0x13,0x12,0x11,0x10,
	0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,
};

const int trianglewave[256]= //triangle wave
{
	0x000,0x020,0x040,0x060,0x080,0x0a0,0x0c0,0x0e0,0x100,0x120,0x140,0x160,0x180,0x1a0,0x1c0,0x1e0,
	0x200,0x220,0x240,0x260,0x280,0x2a0,0x2c0,0x2e0,0x300,0x320,0x340,0x360,0x380,0x3a0,0x3c0,0x3e0,
	0x400,0x420,0x440,0x460,0x480,0x4a0,0x4c0,0x4e0,0x500,0x520,0x540,0x560,0x580,0x5a0,0x5c0,0x5e0,
	0x600,0x620,0x640,0x660,0x680,0x6a0,0x6c0,0x6e0,0x700,0x720,0x740,0x760,0x780,0x7a0,0x7c0,0x7e0,
	0x800,0x820,0x840,0x860,0x880,0x8a0,0x8c0,0x8e0,0x900,0x920,0x940,0x960,0x980,0x9a0,0x9c0,0x9e0,
	0xa00,0xa20,0xa40,0xa60,0xa80,0xaa0,0xac0,0xae0,0xb00,0xb20,0xb40,0xb60,0xb80,0xba0,0xbc0,0xbe0,
	0xc00,0xc20,0xc40,0xc60,0xc80,0xca0,0xcc0,0xce0,0xd00,0xd20,0xd40,0xd60,0xd80,0xda0,0xdc0,0xde0,
	0xe00,0xe20,0xe40,0xe60,0xe80,0xea0,0xec0,0xee0,0xf00,0xf20,0xf40,0xf60,0xf80,0xfa0,0xfc0,0xfe0,
	0xff0,0xfd0,0xfb0,0xf90,0xf70,0xf50,0xf30,0xf10,0xef0,0xef0,0xeb0,0xe90,0xe70,0xe50,0xe30,0xe10,
	0xdf0,0xdd0,0xdb0,0xd90,0xd70,0xd50,0xd30,0xd10,0xcf0,0xcf0,0xcb0,0xc90,0xc70,0xc50,0xc30,0xc10,
	0xbf0,0xbd0,0xbb0,0xb90,0xb70,0xb50,0xb30,0xb10,0xaf0,0xaf0,0xab0,0xa90,0xa70,0xa50,0xa30,0xa10,
	0x9f0,0x9d0,0x9b0,0x990,0x970,0x950,0x930,0x910,0x8f0,0x8f0,0x8b0,0x890,0x870,0x850,0x830,0x810,
	0x7f0,0x7d0,0x7b0,0x790,0x770,0x750,0x730,0x710,0x6f0,0x6f0,0x6b0,0x690,0x670,0x650,0x630,0x610,
	0x5f0,0x5d0,0x5b0,0x590,0x570,0x550,0x530,0x510,0x4f0,0x4f0,0x4b0,0x490,0x470,0x450,0x430,0x410,
	0x3f0,0x3d0,0x3b0,0x390,0x370,0x350,0x330,0x310,0x2f0,0x2f0,0x2b0,0x290,0x270,0x250,0x230,0x210,
	0x1f0,0x1d0,0x1b0,0x190,0x170,0x150,0x130,0x110,0x0f0,0x0f0,0x0b0,0x090,0x070,0x050,0x030,0x010
};
const int EKG[256]= //EKG wave
{
	73,74,75,75,74,73,73,73,73,72,71,69,68,67,67,67,
	68,68,67,65,62,61,59,57,56,55,55,54,54,54,55,55,
	55,55,55,55,54,53,51,50,49,49,52,61,77,101,132,
	169,207,238,255,254,234,198,154,109,68,37,17,5,
	0,1,6,13,20,28,36,45,52,57,61,64,65,66,67,68,68,
	69,70,71,71,71,71,71,71,71,71,72,72,72,73,73,74,
	75,75,76,77,78,79,80,81,82,83,84,86,88,91,93,96,
	98,100,102,104,107,109,112,115,118,121,123,125,
	126,127,127,127,127,127,126,125,124,121,119,116,
	113,109,105,102,98,95,92,89,87,84,81,79,77,76,75,
	74,73,72,70,69,68,67,67,67,68,68,68,69,69,69,69,
	69,69,69,70,71,72,73,73,74,74,75,75,75,75,75,75,
	74,74,73,73,73,73,72,72,72,71,71,71,71,71,71,71,
	70,70,70,69,69,69,69,69,70,70,70,69,68,68,67,67,
	67,67,66,66,66,65,65,65,65,65,65,65,65,64,64,63,
	63,64,64,65,65,65,65,65,65,65,64,64,64,64,64,64,
	64,64,65,65,65,66,67,68,69,71,72,73
};

void Init_DAC(void)
{
	SET_BIT(RCC->APB1ENR, RCC_APB1Periph_DAC);

	DAC_InitTypeDef	DAC_InitStructure;

	DAC_StructInit(&DAC_InitStructure);

	DAC_DeInit();
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel_1,ENABLE);
}

void DAC_Handler(void)
{
	static unsigned char x=0;

	if(TimerUStatus(eTimerUS_Dac) == TIMER_END)
	{
		TimerUStart(eTimerUS_Dac, 1);
		DAC_SetChannel1Data(DAC_Align_12b_R, sinewave[x++]);
//		DAC_SetChannel1Data(DAC_Align_12b_R, rewsawtoothwave[x++]);
//		DAC_SetChannel1Data(DAC_Align_12b_R, sawtoothwave[x++]);
//		DAC_SetChannel1Data(DAC_Align_12b_R, trianglewave[x++]);
//		DAC_SetChannel1Data(DAC_Align_12b_R, EKG[x++]*10);
	}
}
