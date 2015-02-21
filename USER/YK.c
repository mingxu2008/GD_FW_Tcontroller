#include "stm32f10x.h"
#include "YK.h"
#include "mb.h"
extern u16 usRegHoldingBuf[9];
u8  TxBuffer2[TxBufferSize2];
u8  RxBuffer2[RxBufferSize2];
__IO u8 TxCounter2 = 0x00;
__IO u8 RxCounter2 = 0x00;
u8 NumofTX = 0;
u8 NumofRX = 0;

uint16_t Air_Data[9];

void Serial2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USARTy Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

	//GPIOA2 USART2_Tx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIOA3 USART2_Rx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //浮动输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //设定USART1 中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = 9600;            //只修改波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  //串口初始化
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
  //使能USART2
  USART_Cmd(USART2, ENABLE);
  
}


void Enable_TxRx(void)
{
	u32 Delay_i = 600000;
	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
 	 USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	 do{
	 	
	 	}while((RxCounter2 < NumofRX) && ((--Delay_i )> 0));

}

u8 Send_Air(u8 cmd,u8 DataH, u8 DataL)
{
	u8 tmp_return;
	u8 tmp_xor = 0x00;
	TxCounter2 = 0;
	TxBuffer2[TxCounter2++] = cmd;		//CMD
	TxBuffer2[TxCounter2++] = DataH;	//DataH
	TxBuffer2[TxCounter2++] = DataL;	//DataL	
	TxBuffer2[TxCounter2++] = 0x08;		//TM

	tmp_xor = TxBuffer2[0] ^TxBuffer2[1]^TxBuffer2[2]^TxBuffer2[3];

	TxBuffer2[TxCounter2++] = tmp_xor;		//XOR
	
	NumofTX = TxCounter2;
	TxCounter2 = 0;
	
	NumofRX = 1;
	RxCounter2 = 0;

	Enable_TxRx();
	
	if(RxBuffer2[0] == YK_SUCCESS)
	 	tmp_return = 0;			//success
	else
	 	tmp_return = 1;			//fail
	 	RxBuffer2[0] = 0x00;
	return tmp_return;
	
}

u8 ee_temp[2];
void Air_Init(void)
{
	u8 tmp_flag = 0;
	usRegHoldingBuf[AIR_CMD_TYPE] = 85;	//000~999
	usRegHoldingBuf[AIR_CMD_ONOFF] = 0Xff;	//0x00 off, 0xff on
	usRegHoldingBuf[AIR_CMD_MODE] = 0X00;	//0x00 auto,0x01 cold,0x02 dehumidification,0x03 sendwind,0x04 warm
	usRegHoldingBuf[AIR_CMD_TEMP] = 20; //0x10~0x1e ,16-31 degree
	usRegHoldingBuf[AIR_CMD_WS] = 0X00; //0x00 auto,0x01 1 speed,0x02 2 speed, 0x03 3speed
	usRegHoldingBuf[AIR_CMD_WD] = 0X00; //0x00 auto, 0x01 manual


	tmp_flag = Send_Air(AIR_CMD_TYPE,(u8)(0xff&(usRegHoldingBuf[AIR_CMD_TYPE]>>8)),(u8)usRegHoldingBuf[AIR_CMD_TYPE]);
	if(tmp_flag == 0)
		{
			Air_Data[AIR_CMD_TYPE] = usRegHoldingBuf[AIR_CMD_TYPE];
			ee_temp[0] = u8(Air_Data[AIR_CMD_TYPE]);
			ee_temp[1] = u8(Air_Data[AIR_CMD_TYPE]>>8);
			sEE_WriteBuffer(&ee_temp[0],AIR_CMD_TYPE,1);
			sEE_WriteBuffer(&ee_temp[1],AIR_CMD_TYPE+1,1);
		}
	
	tmp_flag = Send_Air(AIR_CMD_ONOFF,(u8)usRegHoldingBuf[AIR_CMD_ONOFF],0x08);
	if(tmp_flag == 0)
		{
			Air_Data[AIR_CMD_ONOFF] = usRegHoldingBuf[AIR_CMD_ONOFF];	//0x00 off, 0xff on
		}
	
	tmp_flag = Send_Air(AIR_CMD_MODE,(u8)usRegHoldingBuf[AIR_CMD_MODE],0x08);
	if(tmp_flag == 0)
		{
			Air_Data[AIR_CMD_MODE] = usRegHoldingBuf[AIR_CMD_MODE];	//0x00 auto,0x01 cold,0x02 dehumidification,0x03 sendwind,0x04 warm
		}
	
	tmp_flag = Send_Air(AIR_CMD_TEMP,(u8)usRegHoldingBuf[AIR_CMD_TEMP],0x08);
	if(tmp_flag == 0)
		{
			Air_Data[AIR_CMD_TEMP] = usRegHoldingBuf[AIR_CMD_TEMP]; //0x10~0x1e ,16-31 degree
		}
	
	tmp_flag = Send_Air(AIR_CMD_WS,(u8)usRegHoldingBuf[AIR_CMD_WS],0x08);
	if(tmp_flag == 0)
		{
			Air_Data[AIR_CMD_WS] = usRegHoldingBuf[AIR_CMD_WS]; //0x00 auto,0x01 1 speed,0x02 2 speed, 0x03 3speed	
		}
	
	tmp_flag = Send_Air(AIR_CMD_WD,(u8)usRegHoldingBuf[AIR_CMD_WD],0x08);
	if(tmp_flag == 0)
		{
			Air_Data[AIR_CMD_WD] = usRegHoldingBuf[AIR_CMD_WD]; //0x00 auto, 0x01 manual
		}
	
}


void Air_Sub_Poll(void)
{
			u8 tmp_flag;
			tmp_flag = Send_Air(AIR_CMD_ONOFF,(u8)usRegHoldingBuf[AIR_CMD_ONOFF],0x08);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_ONOFF] = usRegHoldingBuf[AIR_CMD_ONOFF];	//0x00 off, 0xff on
			}
			tmp_flag = Send_Air(AIR_CMD_MODE,(u8)usRegHoldingBuf[AIR_CMD_MODE],0x08);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_MODE] = usRegHoldingBuf[AIR_CMD_MODE];	//0x00 off, 0xff on
			}			
			tmp_flag = Send_Air(AIR_CMD_WS,(u8)usRegHoldingBuf[AIR_CMD_WS],0x08);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_WS] = usRegHoldingBuf[AIR_CMD_WS];	//0x00 off, 0xff on
			}
			tmp_flag = Send_Air(AIR_CMD_WD,(u8)usRegHoldingBuf[AIR_CMD_WD],0x08);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_WD] = usRegHoldingBuf[AIR_CMD_WD];	//0x00 off, 0xff on
			}
}

void Air_Poll(void)
{
	u8 tmp_flag;

	if(Air_Data[AIR_CMD_TYPE] != usRegHoldingBuf[AIR_CMD_TYPE])
		{
			tmp_flag = Send_Air(AIR_CMD_TYPE,(u8)(0xff&(usRegHoldingBuf[AIR_CMD_TYPE]>>8)),(u8)usRegHoldingBuf[AIR_CMD_TYPE]);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_TYPE] = usRegHoldingBuf[AIR_CMD_TYPE];
			}

			Air_Sub_Poll();
		}

	if(Air_Data[AIR_CMD_ONOFF] != usRegHoldingBuf[AIR_CMD_ONOFF])
		{
			Air_Sub_Poll();
		}
	if(Air_Data[AIR_CMD_MODE] != usRegHoldingBuf[AIR_CMD_MODE])
		{
			tmp_flag = Send_Air(AIR_CMD_MODE,(u8)usRegHoldingBuf[AIR_CMD_MODE],0x08);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_MODE] = usRegHoldingBuf[AIR_CMD_MODE];	//0x00 off, 0xff on
			}
		}
	if(Air_Data[AIR_CMD_TEMP] != usRegHoldingBuf[AIR_CMD_TEMP])
		{
			tmp_flag = Send_Air(AIR_CMD_TEMP,(u8)usRegHoldingBuf[AIR_CMD_TEMP],0x08);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_TEMP] = usRegHoldingBuf[AIR_CMD_TEMP];	//0x00 off, 0xff on
			}
		}
	if(Air_Data[AIR_CMD_WS] != usRegHoldingBuf[AIR_CMD_WS])
		{
			tmp_flag = Send_Air(AIR_CMD_WS,(u8)usRegHoldingBuf[AIR_CMD_WS],0x08);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_WS] = usRegHoldingBuf[AIR_CMD_WS];	//0x00 off, 0xff on
			}
		}
	if(Air_Data[AIR_CMD_WD] != usRegHoldingBuf[AIR_CMD_WD])
		{
			tmp_flag = Send_Air(AIR_CMD_WD,(u8)usRegHoldingBuf[AIR_CMD_WD],0x08);
			if(tmp_flag == 0)
			{
				Air_Data[AIR_CMD_WD] = usRegHoldingBuf[AIR_CMD_WD];	//0x00 off, 0xff on
			}
		}

}




