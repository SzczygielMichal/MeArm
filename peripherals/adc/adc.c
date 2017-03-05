#include "stm32f10x.h"
#include "..\peripherals\adc\adc.h"
#include "..\peripherals\timer\timer.h"
#include "..\setup\setup.h"
#include "..\StdPeriph_Driver\inc\stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

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

/**
  * @brief  This function configures the DMA to store the result of an ADC sequence.
  *         The conversion results are stored in ADC_Control.PWM.Rtab[MAX_R_SAMPLE] array.
  * @param  None
  * @retval None
  */

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

//	ADC_SoftwareStartInjectedConv(ADC1);
}


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

	ADC_InitStructure.ADC_Mode = ENABLE;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = 0;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 4;

	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_28Cycles5);
}
