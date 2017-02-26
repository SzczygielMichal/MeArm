#ifndef ADC_H_
  #define ADC_H_

  /// Liczba probek zbierana przez DMA do wyznaczenia 600Hz na linii R (min to 14 sampli przy probkowaniu 8400Hz)
  #define MAX_R_SAMPLE  128//56


  #define TEMP_CONST_FACTOR 0.030517578
  #define AVG_SLOPE         1.61
                           
  #define ADC_CONST_FACTOR 0.000805664
  #define ADC_CONST_MUL_TO_REAL 8.5

  #define ADC_REAL_COEFF 0.006848144    /// coefficient:: gain & const_factor = real_value
  
  

  #define VREFINT_CAL (*(int16_t *) 0x1FF800F8)   /// Pod tym adresem jest wartosc:1677 -> 0x68D
  #define TSENSE_CAL1 (*(int16_t *) 0x1FF800FA)   /// Pod tym adresem jest wartosc: 667 -> 0x29B
  #define TSENSE_CAL2 (*(int16_t *) 0x1FF800FE)   /// Pod tym adresem jest wartosc: 859 -> 0x35B
  #define T_CAL1    30
  #define T_CAL2    110
  #define AVG_SHIFT 4
  #define TEMP_NORM(temperature,vref)   (T_CAL2-T_CAL1) * 10 * \
                                        ( ( (int32_t) temperature >> AVG_SHIFT ) * VREFINT_CAL - TSENSE_CAL1 * (int32_t)( vref >> AVG_SHIFT) )\
                                      / ( ( (int32_t) TSENSE_CAL2 - (int32_t) TSENSE_CAL1) * (int32_t)( vref >> AVG_SHIFT) ) \
                                      + T_CAL1 *10
                                         
  #define ADC_INJECT_DELAY  200
  


enum eADC_Regular
{
  eADCr_DC_RSAG,
  eADCr_MaxChannel,
};

enum eADC_Injected
{
  eADCi_DC_RP_SAG,
  eADCi_Iwew,
  eADCi_Uwew,
  eADCi_UZas1,
//  eADC_Temp,
//  eADC_Vref,
  eADCi_MaxChannel,
};


struct sADC_Control
{
  unsigned char Step;
  struct
  {
    unsigned int Value[eADCi_MaxChannel];

  }ADC_1;

  struct
  {
    unsigned int Iuc[3]; // sample z prądem
    unsigned int Up[3]; // sample z napieciem na linii P
    unsigned char Counter;
  }Samples;
  
  struct
  {
    float Current;
    float PLineVoltage;
  }RealValue;

  struct
  {
    unsigned char RStep;
    volatile short Rtab[MAX_R_SAMPLE];
  }PWM;

  
};

extern struct sADC_Control ADC_Control;


void Init_ADC(void);

void StartConversion_ADC(void);

float ADC_Read_Voltage(enum eADC_Injected ADC_Channel);

void ADC_Handler(void);

void ADC1_IRQHandler(void);

#endif


