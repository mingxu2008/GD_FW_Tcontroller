#include "stm32f10x.h"
#include "sys.h"
#include "mb.h"
#include "mbutils.h"
#include "T_Sensor.h"
#include "Led.h"
#include "Timer2.h"
#include "Switch.h"
#include "eeprom.h"

/*****************/

#include "delay.h"
#include "oled.h"

#include "T_GPIO.h"
//#include "bmp.h"

//输入寄存器起始地址
#define REG_INPUT_START       0x0000
//输入寄存器数量
#define REG_INPUT_NREGS       8
//保持寄存器起始地址
#define REG_HOLDING_START     0x0000
//保持寄存器数量
#define REG_HOLDING_NREGS     9

//线圈起始地址
#define REG_COILS_START       0x0000
//线圈数量
#define REG_COILS_SIZE        16

//开关寄存器起始地址
#define REG_DISCRETE_START    0x0000
//开关寄存器数量
#define REG_DISCRETE_SIZE     16


/* Private variables ---------------------------------------------------------*/
//输入寄存器内容
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008};
//寄存器起始地址
uint16_t usRegInputStart = REG_INPUT_START;

//保持寄存器内容
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000,0X9000};
//保持寄存器起始地址
uint16_t usRegHoldingStart = REG_HOLDING_START;

//线圈状态
uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0x00,0x00};
//开关输入状态
uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8] = {0x00,0x00};
 int main(void)
 {   
 	uint32_t t1 = 0;
	uint8_t button = 0;
	uint8_t oled_flag =0;
	uint16_t nn = 1;
	 
		T_GPIO_Init();
		ADC1_Init();
		delay_init();	    	 //delay	  
		OLED_Init();			//oled  
		OLED_Clear();
	 
	 	GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_8 | GPIO_Pin_9);
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
	 
	 while(1)
	 {
	 	ADC1_Poll();
		 if(t1 != 0)
		 {
				t1--;
		 }
		 
		// if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0)
		 {
			 if(t1 == 0)
			{
				delay_ms(1);
		//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0)
				{
					 t1 = 360000;
					 button = 1;
				}
			}
		 }
		 
		 if(button == 1)
		 {
			button = 0;
			 switch(oled_flag){
				 case 0: 
								OLED_ShowSymbol(0,0,0);
								OLED_ShowBig(0,2,11);
					 			OLED_ShowBig(16,2,Temp_True[0]/100);
								OLED_ShowBig(48,2,Temp_True[0]%100/10);
								OLED_ShowBig(80,2,10);
								OLED_ShowBig(96,2,Temp_True[0]%100%10);
								OLED_ShowSymbol(32,0,1);
								OLED_ShowSymbol(64,0,2);
								OLED_ShowSymbol(96,0,3);
								
							//OLED_ShowSymbol(0,2,1);
								//OLED_ShowSymbol(0,4,2);
								//OLED_ShowSymbol(0,6,3);
								oled_flag = 1;
								break;
				 case 1:
								OLED_ShowSymbol(0,0,1);
					 			OLED_ShowBig(16,2,Temp_True[1]/100);
								OLED_ShowBig(48,2,Temp_True[1]%100/10);
								OLED_ShowBig(80,2,10);
								OLED_ShowBig(96,2,Temp_True[1]%100%10);
								
								//OLED_ShowSymbol(0,2,1);
								//OLED_ShowSymbol(0,4,2);
								//OLED_ShowSymbol(0,6,3);
								oled_flag= 0;
								break;
				 default:
								break;
			 }

		 }
		 
	 }
	 
	 
#if 0	 
	while(1) 
	{		
		OLED_Clear();
		//GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 |GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
/*	
		GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_8 | GPIO_Pin_9);
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
*/		
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		delay_ms(1000);
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		delay_ms(1000);
	/*
		GPIO_ResetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_8 | GPIO_Pin_9);
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	*/
		delay_ms(1000);
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		delay_ms(1000);
	//	OLED_ShowCHinese(0,0,0);//?
		OLED_ShowBig(16,0,2);
		OLED_ShowBig(48,0,5);
		OLED_ShowBig(80,0,10);
		OLED_ShowBig(96,0,6);
		OLED_ShowSymbol(0,0,0);
		OLED_ShowSymbol(0,2,1);
		OLED_ShowSymbol(0,4,2);
		OLED_ShowSymbol(0,6,3);
		
		
		delay_ms(1000);
	}
#endif
	
	#if 0 
 	Switch_Init();
 	Serial2_Init();
  eMBInit(MB_RTU, Get_Address(), 0x01, 9600, MB_PAR_NONE); //初始化 RTU模式 从机地址为1 USART1 9600 无校验  
	Air_Init();
	ADC1_Init();
	Led_Init();
	Timer2_Init();
	sEE_Init();
	sEE_WriteBuffer(&eeww,1,1);
	sEE_ReadBuffer(&eeaa,(uint16_t)1,&nn);
	eMBEnable(); //启动FreeModbus 
  while(1)
	{ 
		eMBPoll(); 
		Air_Poll();
		ADC1_Poll(&usRegHoldingBuf[HOLD_REG_TEMP]);
	}
#endif
 }



 /**
  * @brief  输入寄存器处理函数，输入寄存器可读，但不可写。
  * @param  pucRegBuffer  返回数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  * @retval eStatus       寄存器状态
  */
eMBErrorCode 
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
  eMBErrorCode    eStatus = MB_ENOERR;
  int16_t         iRegIndex;
  
  //查询是否在寄存器范围内
  //为了避免警告，修改为有符号整数
  if( ( (int16_t)usAddress >= REG_INPUT_START ) \
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
  {
    //获得操作偏移量，本次操作起始地址-输入寄存器的初始地址
    iRegIndex = ( int16_t )( usAddress - usRegInputStart );
    //逐个赋值
    while( usNRegs > 0 )
    {
      //赋值高字节
      *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );
      //赋值低字节
      *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );
      //偏移量增加
      iRegIndex++;
      //被操作寄存器数量递减
      usNRegs--;
    }
  }
  else
  {
    //返回错误状态，无寄存器  
    eStatus = MB_ENOREG;
  }

  return eStatus;
}

/**
  * @brief  保持寄存器处理函数，保持寄存器可读，可读可写
  * @param  pucRegBuffer  读操作时--返回数据指针，写操作时--输入数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  *         eMode         操作方式，读或者写
  * @retval eStatus       寄存器状态
  */
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //偏移量
  int16_t         iRegIndex;
  
  //判断寄存器是不是在范围内
  if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
     && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
  {
    //计算偏移量
    iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );
    
    switch ( eMode )
    {
      //读处理函数  
      case MB_REG_READ:
        while( usNRegs > 0 )
        {
          *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
          *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
          iRegIndex++;
          usNRegs--;
        }
        break;

      //写处理函数 
      case MB_REG_WRITE:
        while( usNRegs > 0 )
        {
          usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
          usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
          iRegIndex++;
          usNRegs--;
        }
        break;
     }
  }
  else
  {
    //返回错误状态
    eStatus = MB_ENOREG;
  }
  
  return eStatus;
}


/**
  * @brief  线圈寄存器处理函数，线圈寄存器可读，可读可写
  * @param  pucRegBuffer  读操作---返回数据指针，写操作--返回数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  *         eMode         操作方式，读或者写
  * @retval eStatus       寄存器状态
  */
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //寄存器个数
  int16_t         iNCoils = ( int16_t )usNCoils;
  //寄存器偏移量
  int16_t         usBitOffset;

  //检查寄存器是否在指定范围内
  if( ( (int16_t)usAddress >= REG_COILS_START ) &&
        ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
  {
    //计算寄存器偏移量
    usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
    switch ( eMode )
    {
      //读操作
      case MB_REG_READ:
        while( iNCoils > 0 )
        {
          *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
                                          ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
          iNCoils -= 8;
          usBitOffset += 8;
        }
        break;

      //写操作
      case MB_REG_WRITE:
        while( iNCoils > 0 )
        {
          xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
                        ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
                        *pucRegBuffer++ );
          iNCoils -= 8;
        }
        break;
    }

  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //操作寄存器个数
  int16_t         iNDiscrete = ( int16_t )usNDiscrete;
  //偏移量
  uint16_t        usBitOffset;

  //判断寄存器时候再制定范围内
  if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
        ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
  {
    //获得偏移量
    usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );
    
    while( iNDiscrete > 0 )
    {
      *pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
                                      ( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
      iNDiscrete -= 8;
      usBitOffset += 8;
    }

  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}
