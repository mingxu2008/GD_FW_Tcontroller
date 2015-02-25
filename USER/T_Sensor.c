#include "T_Sensor.h"
#include "stm32f10x.h"

__IO uint16_t ADCConvertedValue[2];

__IO uint32_t Temp[2];
__IO uint32_t Temp_Ave[2];
__IO int32_t Temp_True[2];
__IO uint32_t Temp_i;
void ADC1_Init(void)
{

ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

	  /* ADCCLK = PCLK2/4 */
 	 RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	  
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);	  

  /* Configure PA0 PB0 (ADC Channel14) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  

    /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_239Cycles5);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

static void temp_list(u8 i)
{
	if(Temp_Ave[i] <796 )
		{
			Temp_True[i] = ( 380*Temp_Ave[i]-358600)/1000;
		}
	else if(Temp_Ave[i] <1192)
		{
			Temp_True[i] = ( 275*Temp_Ave[i]-272800)/1000;
		}
	else if(Temp_Ave[i] <2536)
		{
			Temp_True[i] = ( 228*Temp_Ave[i]-213700)/1000;
		}
	else if(Temp_Ave[i] <3058)
		{
			Temp_True[i] = ( 288*Temp_Ave[i]-366500)/1000;
		}
	else if(Temp_Ave[i] <3424)
		{
			Temp_True[i] = ( 410*Temp_Ave[i]-741000)/1000;
		}
	else if(Temp_Ave[i] <3470)
		{
			Temp_True[i] = ( 700*Temp_Ave[i]-1731000)/1000;
		}
	else 
		{
			Temp_True[i] = ( 1270*Temp_Ave[i]-3868000)/1000;		
		}
}

void ADC1_Poll(void)
{
	if(Temp_i == SAMPLE_NUM)
		{
			Temp_Ave[0] = Temp[0]/SAMPLE_NUM;
			Temp_Ave[1] = Temp[1]/SAMPLE_NUM;
			temp_list(0);
			temp_list(1);
			
			Temp_i = 0;
			Temp[0] = 0;
			Temp[1] = 0;
		}
	else if(Temp_i > SAMPLE_NUM)
		{
			Temp_i = 0;
			Temp[0] = 0;
			Temp[1] = 0;
		}
	else
		{
			Temp[0] = Temp[0] + ADCConvertedValue[0];
			Temp[1] = Temp[1] + ADCConvertedValue[1];
			Temp_i ++;
		}
	
#if 0	
	int tmp[2];
	tmp[0] = ((int)ADCConvertedValue[0] -1080) *10/36; //need -1080*10/34
	//tmp = ((int)900 -1080) *10/34; //need -1080*10/34
	tmp[1] = ((int)ADCConvertedValue[1] -1080) *10/36; //need -1080*10/34
#endif
}



