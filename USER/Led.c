#include "Led.h"


void Led_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	 /* GPIOD Periph clock enable */
 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	   /* Configure PD0 and PD2 in output pushpull mode */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void Led1_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

void Led1_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

