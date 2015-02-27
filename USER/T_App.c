#include "T_App.h"
#include "T_Sensor.h"
#include "oled.h"
uint16_t  *T_REG = usRegHoldingBuf;

uint32_t Button_t,Mode_t;
uint8_t button_flag;
uint16_t oled_t;	

void Reg_Poll(void)
{
	T_REG[TEMP_1] = Temp_True[0];
	T_REG[TEMP_2] = Temp_True[1];

}



void Relay_Poll(void)
{
	if(T_REG[DEFROST] == ON)						//defrost
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_9);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	}
	if(T_REG[COMPRESSOR] == ON)					//COMPRESSOR
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_8);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	}
	if(T_REG[DEMIST] == ON)						//DEMIST
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	}
	if(T_REG[LIGHT] == ON)						//LIGHT
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	}
	if(T_REG[FAN] == ON)							//FAN
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_3);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	}
	if(T_REG[AUX] == ON)							//AUX
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	}

}

uint16_t MODE_Temp = 0;


void Button_Poll(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0)
	{
				
		if(Button_t == 0)
		{
			delay_ms(1);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0)
			{
				Button_t = D500MS;		//500ms
				button_flag = 1;
			}
		}

	}

	if(button_flag == 1)
	{
		button_flag = 0;
		if(MODE_Temp < MODE_NUM)
		{
			MODE_Temp++;
		}
		else
		{
			MODE_Temp = 0;
		}
		Mode_t = D10S;
		
		
	}

}

void Mode_Poll(void)
{
	if(Mode_t == 0)
	{
		T_REG[RUNNING_MODE] = MODE_Temp;
	}

}

void Oled_Poll(void)
{
	int32_t T_Temp;
	if(oled_t != 0)
	{
		return;
	}
	if(Mode_t == 0)
	{
		if(Temp_True[0]<0)
		{
			OLED_ShowBig(0,2,11);		//'-'
			T_Temp = 0 - Temp_True[0];
		}
		else
		{
			T_Temp = Temp_True[0];
		}
		OLED_ShowBig(16,2,Temp_True[0]/100);
		OLED_ShowBig(48,2,Temp_True[0]%100/10);
		OLED_ShowBig(80,2,10);
		OLED_ShowBig(96,2,Temp_True[0]%100%10);
		OLED_ShowSymbol(0,0,0);
		OLED_ShowChar(16,0,'e');
		OLED_ShowSymbol(28,0,1);
		OLED_ShowSymbol(48,0,2);
		OLED_ShowSymbol(68,0,3);
		OLED_ShowSymbol(88,0,4);
		OLED_ShowSymbol(108,0,5);
	
	}
	else
	{
		OLED_Clear();
		switch(MODE_Temp){
			case COOL_S:
				OLED_ShowString(0,2,"Freezing S");
			case COOL_E:
				OLED_ShowString(0,2,"Freezing E");
			case COOL_H:
				OLED_ShowString(0,2,"Freezing H");
			case COMP_CLOSE:
				OLED_ShowString(0,2,"COMP CLOSE");
			case T_OFF:
				OLED_ShowString(0,2,"T OFF");
			case AUTO_CTR:
				OLED_ShowString(0,2,"AUTO");	
		}
	}

}

#if 0
void Oled_Poll(void)
{
	int32_t T_Temp;
	
	
	if(Temp_True[0]<0)
	{
		OLED_ShowBig(0,2,11);		//'-'
		T_Temp = 0 - Temp_True[0];
	}
	else
	{
		T_Temp = Temp_True[0];
	}
	OLED_ShowBig(16,2,Temp_True[0]/100);
	OLED_ShowBig(48,2,Temp_True[0]%100/10);
	OLED_ShowBig(80,2,10);
	OLED_ShowBig(96,2,Temp_True[0]%100%10);
	
	
	switch(MODE_Temp){
		case COOL_S:
			if(MODE_Temp != T_REG[RUNNING_MODE])
			{
				OLED_ShowSymbol(0,0,0);
			}
			OLED_ShowSymbol(0,0,0);
		
			break;
	}
	
	OLED_ShowChar(16,0,'e');
	OLED_ShowSymbol(28,0,1);
	OLED_ShowSymbol(48,0,2);
	OLED_ShowSymbol(68,0,3);
	OLED_ShowSymbol(88,0,4);
	OLED_ShowSymbol(108,0,5);

}
#endif

void T_Poll(void)
{
	Relay_Poll();
	Reg_Poll();
	Button_Poll();
	Mode_Poll();
	Oled_Poll();
}



