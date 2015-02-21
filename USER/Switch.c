#include "Switch.h"


void Switch_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	 /* GPIOD Periph clock enable */
 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	   /* Configure PD0 and PD2 in output pushpull mode */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

u8 Get_Address(void)
{
	u8 result = 0x00;
	result = (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)<<3) | (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)<<2)  | (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)<<1) | GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) ;
	return result;

}

