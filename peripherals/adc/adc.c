
#include "stm32f10x.h"
#include "..\peripherals\adc\adc.h"
#include "..\peripherals\timer\timer.h"
#include "..\setup\setup.h"
#include "..\inc\stm32f10x_adc.h"
#include "..\inc\stm32f10x_rcc.h"
#include "..\inc\misc.h"

/*------------------------------------------------------------------------*/
//struct sADC_Control ADC_Control;
/*------------------------------------------------------------------------*/
/*--- FUNKCJE PRYWATNE ---------------------------------------------------*/
/*------------------------------------------------------------------------*/
//void ADC_Power_Handler(void);
//void CfgDMAforADC(void);


/**
 *  \brief ADC1_IRQHandler
 */

//void ADC1_IRQHandler(void)
//{
//  unsigned int ADC1_Status = ADC1->SR;

//  NVIC_ClearPendingIRQ(ADC1_IRQn);
  
  
  /// Injected Channel
//  if((ADC1_Status & ADC_SR_JEOC ) != 0) /// bit czyszczonypodczas odczytu z ADC1->DR
//  {
//    ADC_Control.ADC_1.Value[eADCi_DC_RP_SAG]= ADC1->JDR1;
//    ADC_Control.ADC_1.Value[eADCi_Iwew]   = ADC1->JDR2;
//    ADC_Control.ADC_1.Value[eADCi_Uwew]   = ADC1->JDR3;
//    ADC_Control.ADC_1.Value[eADCi_UZas1]  = ADC1->JDR4;
    
//  CLEAR_BIT(ADC1->SR,ADC_SR_JEOC);
//  }
  
//  if((ADC1_Status & ADC_SR_OVR) != 0)
//  {
//    ADC1->SR |= ADC_SR_OVR; /* Clear the pending bit */
    
//    DMA1_Channel1->CCR &= (uint32_t)(~DMA_CCR1_EN); /* Disable DMA Channel 1 to write in CNDTR*/
//    DMA1_Channel1->CNDTR = MAX_R_SAMPLE; /* Reload the number of DMA tranfer to be performs on DMA channel 1 */
//    DMA1_Channel1->CCR |= DMA_CCR1_EN; /* Enable again DMA Channel 1 */
    
// }

  //CLEAR_REG(ADC1->SR);
//}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*void ADC_Handler(void)
{  
  switch(ADC_Control.Step)
  { 
    default: // inicjalizacja 
      if( ADC1->SR & ADC_SR_RCNR )
        break;
      if( ADC1->SR & ADC_SR_JCNR ) 
        break;
      
      if((ADC1->SR & ADC_SR_ADONS) == 0 )
        break;
            
      TimerUStart(eTimerUS_Adc, 4);
      TimerLowSpeedStart(eTimerLowSpeed_Test, 5);
        
      StartConversion_ADC(); 
      ADC_Control.Step = 1;
      
      
      break;
    
    case 1:
      if(TimerUStatus(eTimerUS_Adc) == TIMER_END)
      {
        if(ADC1->SR & ADC_SR_JCNR )
          break;
        TimerUStart(eTimerUS_Adc, ADC_INJECT_DELAY); // 100 x 100us = 100ms pomiar ADC
        StartConversion_ADC();
      }
      
      if(TimerLowSpeedStatus(eTimerLowSpeed_Test) == TIMER_END)
      {
        if(ADC1->SR & ADC_SR_RCNR )
          break;
        TimerLowSpeedStart(eTimerLowSpeed_Test, 20); // 500ms
        ADC_Power_Handler();
      }
      break;
  }
}
*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*
void Init_ADC(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;

	NVIC_InitTypeDef NVICC;

	NVICC.NVIC_IRQChannel = ADC1_IRQn;
	NVICC.NVIC_IRQChannelPreemptionPriority = 2;
	NVICC.NVIC_IRQChannelSubPriority = 0;
	NVICC.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICC);

	// Enable The HSI (16Mhz) //
	RCC_HSICmd(ENABLE);
  
	// Check that HSI oscillator is ready //
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	// Set ADC Prescaler Div2 //
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInit(&ADC_CommonInitStruct);
 
	// ADC1 Configuration //
	SET_BIT(  RCC->APB2RSTR, RCC_APB2RSTR_ADC1RST); // RESET peripheral ADC1 set
	CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_ADC1RST); // RESET peripheral ADC1 clear
  
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);      // Turn ON clock for ADC1

	//--- ADC1 -------------------------------
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;            /// For REGULAR and INJECTED channels !!!
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T4_TRGO;        // not used for trigger
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = eADCr_MaxChannel;
	ADC_Init(ADC1, &ADC_InitStructure);
    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14,  1, ADC_SampleTime_24Cycles);    // R_Line  96

	ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);

	ADC_InjectedDiscModeCmd(ADC1, ENABLE);
	ADC_InjectedSequencerLengthConfig(ADC1, 4);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_18, 1, ADC_SampleTime_24Cycles);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_8,  2, ADC_SampleTime_24Cycles);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_9,  3, ADC_SampleTime_24Cycles);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_15, 4, ADC_SampleTime_24Cycles);
  
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T4_TRGO);
	ADC_ExternalTrigInjectedConvEdgeConfig(ADC1, ADC_ExternalTrigInjecConvEdge_None);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);

	// Enable ADC1 //
	ADC_Cmd(ADC1, ENABLE);
    
	// Wait until the ADC1 is ready //
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{
	}

	//  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);  // przerwanie dla injected channel
	ADC_ITConfig(ADC1, ADC_IT_OVR,  ENABLE);

	NVIC_EnableIRQ(ADC1_IRQn);

	//  Konfiguracja DMA dla obslugi ADC //
	CfgDMAforADC();
}
*/

/**
  * @brief  This function configures the DMA to store the result of an ADC sequence.
  *         The conversion results are stored in ADC_Control.PWM.Rtab[MAX_R_SAMPLE] array.
  * @param  None
  * @retval None
  */

//__INLINE void CfgDMAforADC(void)
//{
	/* (1) Enable the peripheral clock on DMA */
	/* (2) Enable DMA transfer on ADC - DMACFG is kept at 0 for one shot mode */
	/* (3) Configure the peripheral data register address */
	/* (4) Configure the memory address */
	/* (5) Configure the number of DMA tranfer to be performs on DMA channel 1 */
	/* (6) Configure increment, size and interrupts */
  
//	/*(1)*/ SET_BIT(RCC->AHBENR, RCC_AHBENR_DMA1EN);
//	/*(2)*/ ADC1->CR2 |= (uint32_t)ADC_CR2_DMA;                   // ADC_DMACmd(ADC1, ENABLE);
  
//	/*(3)*/ DMA1_Channel1->CPAR =  (int)(&ADC1->DR);           // peryferium
//	/*(4)*/ DMA1_Channel1->CMAR =  (int)(ADC_Control.PWM.Rtab);// pamiec
//	/*(5)*/ DMA1_Channel1->CNDTR = MAX_R_SAMPLE;
//	/*(6)*/ DMA1_Channel1->CCR |= DMA_CCR1_MINC | DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0 | DMA_CCR1_TEIE | DMA_CCR1_TCIE    ;///| DMA_CCR1_CIRC;
  
//	/*(7)*/ DMA1_Channel1->CCR |= DMA_CCR1_EN;          /* (7) Enable DMA Channel 1 */

	/* Configure NVIC for DMA */
//	/*(8)*/ NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);   /* (8) Clear Pending IRQ on DMA */
//	/*(9)*/ NVIC_EnableIRQ(DMA1_Channel1_IRQn);         /* (9) Enable Interrupt on DMA  */
//	/*(A)*/ NVIC_SetPriority(DMA1_Channel1_IRQn,1);     /* (A) Set priority for DMA     */
//}

/*
void StartConversion_ADC(void)
{
	unsigned int temp;
  
	ADC_Control.Samples.Iuc[ADC_Control.Samples.Counter] = ADC_Control.ADC_1.Value[eADCi_Iwew];
	ADC_Control.Samples.Up[ADC_Control.Samples.Counter]  = ADC_Control.ADC_1.Value[eADCi_DC_RP_SAG];
	ADC_Control.Samples.Counter++;

	if(ADC_Control.Samples.Counter == 3) ADC_Control.Samples.Counter = 0;
  
	temp  = ADC_Control.Samples.Iuc[0];
	temp += ADC_Control.Samples.Iuc[1];
	temp += ADC_Control.Samples.Iuc[2];
	temp /= 3;
  
	ADC_Control.RealValue.Current = ((temp * ADC_CONST_FACTOR)/0.0114);

	temp  = ADC_Control.Samples.Up[0];
	temp += ADC_Control.Samples.Up[1];
	temp += ADC_Control.Samples.Up[2];
	temp /= 3;

	ADC_Control.RealValue.PLineVoltage = temp * ADC_REAL_COEFF /2.4;

	ADC_SoftwareStartInjectedConv(ADC1);
}
*/


/*
float ADC_Read_Voltage(enum eADC_Injected ADC_Channel)
{
//  if(ADC_Channel == eADC_Temp)
//  {
//    return ((ADC_Control.ADC_1.Value[eADC_Temp] * TEMP_CONST_FACTOR)-40);
//  }
  
  if(ADC_Channel < eADCi_MaxChannel )
    return ADC_Control.ADC_1.Value[ADC_Channel] * ADC_CONST_FACTOR;
  else
    return 0;
}
*/

/*
void Init_ADC(void)
{
	// Wӹczenie zasilania dla ADC 1
	SET_BIT(RCC->APB2ENR, RCC_APB2Periph_ADC1); // Wӹczenie zegara ADC

	// Konfiguracja ADC1
	RCC_HSICmd(ENABLE);
	while (!RCC_GetFlagStatus(RCC_FLAG_HSIRDY));

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = 0;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;

	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_24Cycles);
}
*/

