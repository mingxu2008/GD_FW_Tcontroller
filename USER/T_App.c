#include "T_App.h"
#include "T_Sensor.h"
#include "oled.h"
#include "T_RTC.h"
uint16_t  *T_REG = usRegHoldingBuf;

uint32_t Button_t,Mode_t;
uint8_t button_flag;
uint16_t oled_t;	
uint16_t cool_m;

void Temp_Poll(void)
{
	T_REG[TEMP_1] = Temp_True[0] + T_REG[TEMP_OFFSET1];			//TEMP = TEMP_TRUE + TEMP_OFFSET
	T_REG[TEMP_2] = Temp_True[1] + T_REG[TEMP_OFFSET2];

}

__IO uint16_t comp_i = 0;
__IO uint8_t comp_flag = 0;

void Relay_Poll(void)
{
	if(comp_flag == 1)		//comp delay
	{
		comp_flag = 0;
			if(T_REG[COMPRESSOR] == ON)					//COMPRESSOR
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		}
	}
	if(T_REG[DEFROST] == ON)						//defrost
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_9);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
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
	if(T_REG[DEMIST] == ON)						//DEMIST
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
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
	//button
	if( Mode_t == 0)
	{

	}
	else if(Mode_t < 100)
	{

	}
	else if(Mode_t < 200)
	{
		T_REG[RUNNING_MODE] = MODE_Temp;
		Mode_t = 100;
	}
	else
	{
	
	}
}

void Cool_Mode_Poll()
{

	//normal poll
	if(T_REG[RUNNING_MODE] == COOL_S)
	{
		T_REG[COOL_MODE] = COOL_S;
	}
	else if(T_REG[RUNNING_MODE] == COOL_E)
	{
		T_REG[COOL_MODE] = COOL_E;
	}
	else if(T_REG[RUNNING_MODE] == COOL_H)
	{
		T_REG[COOL_MODE] = COOL_H;
	}
	else
	{
				//T_REG[COOL_MODE]=0xff;	//0xff->COOL_MODE OFF
	}

}

void Oled_Poll(void)
{
	int32_t T_Temp;
if((Mode_t < 100) && (Mode_t > 0))
	{
				OLED_Clear();
				Mode_t = 0;
	}
//oled poll time.	
	if(oled_t != 1000)
	{
		return;
	}
	else
	{
		oled_t = 0;
	}
	
	if(Mode_t == 0)
	{
		//OLED_Clear();
#if 0						//jerry
		if(Temp_True[0]<0)
		{
			OLED_ShowSymbol(32,5,8);	//'-'
			T_Temp = 0 - Temp_True[0];
		}
		else
		{
			T_Temp = Temp_True[0];
		}
#endif
		OLED_ShowSymbol(32,5,8);	//'-'		//jerry
		OLED_ShowBig(48,4,Temp_True[0]/100);
		OLED_ShowBig(72,4,Temp_True[0]%100/10);
	//	OLED_ShowSymbol(88,7,7);
		OLED_Show8x8(96,7,0);
		OLED_ShowBig(104,4,Temp_True[0]%100%10);
			OLED_ShowSymbol(0,0,1);		//freezing		
			OLED_ShowSymbol(28,0,2);	//defrost	
				OLED_ShowSymbol(48,0,3);	//light
				OLED_ShowSymbol(68,0,4);	//fan
				OLED_ShowSymbol(88,0,5);	//demist
				OLED_ShowSymbol(108,0,0);	//aux
#if 0			//jerry
		if(T_REG[COMPRESSOR] == ON )
			OLED_ShowSymbol(0,0,1);		//freezing
		else
			OLED_ShowSymbol(0,0,6);		//' '
		
		if(T_REG[COOL_MODE] == COOL_E  )
			OLED_ShowChar(16,0,'e');	//'e''s''h'
		else if(T_REG[COOL_MODE] == COOL_H)
			OLED_ShowChar(16,0,'h');	//'e''s''h'
		else
			OLED_ShowSymbol(16,0,6);		//' '
		
		if(T_REG[DEFROST] == ON )			
		OLED_ShowSymbol(28,0,2);	//defrost
		else
			OLED_ShowSymbol(28,0,6);		//' '
		
		if(T_REG[LIGHT] == ON )
		OLED_ShowSymbol(48,0,3);	//light
		else
			OLED_ShowSymbol(48,0,6);		//' '
		
		if(T_REG[FAN] == ON )

		OLED_ShowSymbol(68,0,4);	//fan
		else
			OLED_ShowSymbol(68,0,6);		//' '
		
		if(T_REG[DEMIST] == ON )
		OLED_ShowSymbol(88,0,5);	//demist
		else
			OLED_ShowSymbol(88,0,6);		//' '
		
		if(T_REG[AUX] == ON )
		OLED_ShowSymbol(108,0,0);	//aux
		else
			OLED_ShowSymbol(108,0,6);		//' '
#endif	
	}

	else
	{
		//OLED_ShowString(0,0,"<<<<<<<<<<<<<<<<");
		switch(MODE_Temp){
			case COOL_S:
				OLED_ShowString(0,0,"Cooling_S<<<<<<<");break;
			case COOL_E:
				OLED_ShowString(0,0,"Cooling_E<<<<<<<");break;
			case COOL_H:
				OLED_ShowString(0,0,"Cooling_H<<<<<<<");break;
			case DFROST:
				OLED_ShowString(0,0,"Defrost<<<<<<<<<");break;			
			case COMP_CLOSE:
				OLED_ShowString(0,0,"Compclose<<<<<<<");break;
			case T_OFF:
				OLED_ShowString(0,0,"Toff<<<<<<<<<<<<");break;
			case AUTO_CTR:
				OLED_ShowString(0,0,"auto<<<<<<<<<<<<");	break;
			default:
				break;
		}
	}

}

void Sub_Cool(uint8_t m)
{
	int16_t tt;
	T_REG[COOL_MODE] = m;	
	if(m == COOL_S)
	{
		T_REG[SETPOINT] = 4;	//4 degree 
		T_REG[TEMP_DIFF] = 3;	//4-2=2 degree
		T_REG[COMP_DELAY] = 5;	//delay
	}
	else if(m == COOL_E)
	{
		T_REG[SETPOINT] = 4;
		T_REG[TEMP_DIFF] = 2;
		T_REG[COMP_DELAY] = 7;	//delay
	}
	else if(m == COOL_H)
	{
		T_REG[SETPOINT] = 4;
		T_REG[TEMP_DIFF] = 4;
		T_REG[COMP_DELAY] = 3;	//delay
	}
	else
	{}
	

	
	tt = T_REG[SETPOINT]-T_REG[TEMP_DIFF];
	if(T_REG[TEMP_1] > tt)	
	{
		T_REG[COMPRESSOR] = ON;
		T_REG[DEFROST] = OFF;
	}
	else
	{
		T_REG[COMPRESSOR] = OFF;
		T_REG[DEFROST] = OFF;
	}
}

uint16_t defrost_i = 0;
__IO uint8_t defrost_flag = UNSTART;
//uint8_t defrost_finish = 0;

void Sub_Defrost(void)
{
	int16_t tt;
	tt = T_REG[DEFROST_TEMP];
	switch(defrost_flag){
		case UNSTART : 															//Start
			defrost_i = 0;
			defrost_flag = START;
			break;
		case START : 																//Running
			if(T_REG[TEMP_2] < tt)			//defrost on
			{
				T_REG[COMPRESSOR] = OFF;
				T_REG[DEFROST] = ON;
				if(defrost_i >= T_REG[DEFROST_DURA])
				{
					defrost_flag = FINISH;
				}
			}
			else
			{
				defrost_flag = FINISH;
			}
			break;
		case FINISH:
			T_REG[COMPRESSOR] = OFF;		//defrost off
			T_REG[DEFROST] = OFF;
			T_REG[RUNNING_MODE] = T_REG[COOL_MODE];			//recover cool mode
			MODE_Temp = T_REG[RUNNING_MODE];
			defrost_flag = UNSTART;
			break;
		default: break;
	
	
	}
}

void Running_Poll(void)
{
	switch(T_REG[RUNNING_MODE]){
		case COOL_S:			Sub_Cool(COOL_S);
											T_REG[LIGHT] = ON;
											T_REG[FAN] = ON;
											T_REG[DEMIST] = OFF;
											T_REG[AUX] = OFF;	break;
		case COOL_E:			Sub_Cool(COOL_E);
											T_REG[LIGHT] = ON;
											T_REG[FAN] = ON;
											T_REG[DEMIST] = OFF;
											T_REG[AUX] = OFF; break;		
		case COOL_H:			Sub_Cool(COOL_H);
											T_REG[LIGHT] = ON;
											T_REG[FAN] = ON;
											T_REG[DEMIST] = OFF;
											T_REG[AUX] = OFF; break;	
		case DFROST:			Sub_Defrost();
											T_REG[LIGHT] = ON;
											T_REG[FAN] = ON;
											T_REG[DEMIST] = OFF;
											T_REG[AUX] = OFF;
											//T_REG[COOL_MODE] = 0XFF;	
											break;	
		case COMP_CLOSE:	T_REG[COMPRESSOR] = OFF;
											T_REG[DEFROST] = OFF;
											T_REG[LIGHT] = ON;
											T_REG[FAN] = ON;
											T_REG[DEMIST] = OFF;
											T_REG[AUX] = OFF;
										//	T_REG[COOL_MODE] = 0XFF;	
											break;	
		case T_OFF:				T_REG[COMPRESSOR] = OFF;
											T_REG[DEFROST] = OFF;
											T_REG[LIGHT] = OFF;
											T_REG[FAN] = OFF;
											T_REG[DEMIST] = OFF;
											T_REG[AUX] = OFF;
										//	T_REG[COOL_MODE] = 0XFF;	
											break;	
		case AUTO_CTR:		//T_REG[COMPRESSOR] = ON;
											//T_REG[COOL_MODE] = 0XFF;	break;	
		default:	break;
	}

}

void T_Init(void)
{
	T_REG[T_RTC_CFG] = OFF;		//UN SET VALUE
	T_REG[DEFROST_DURA] = 60;	//defrost duration 60s
	T_REG[DEFROST_TEMP] = 100;//defrost tempurature 10 degree.
	//T_REG[T_RTC_CFG2] = 0XFFFF;
}
uint16_t warn_i = 0;
uint8_t warn_flag = 0;
void Warn_Poll(void)
{
	if( warn_flag == 0 )
	{
		return;
	}
	else
	{
		warn_flag = 0;
	}
	if(T_REG[TEMP_1] > T_REG[WARN_TEMP])
	{
		T_REG[WARNING] = ON;
	}
	else
	{
		T_REG[WARNING] = OFF;
	}
}

void T_Poll(void)
{
	Relay_Poll();
	Temp_Poll();
	Button_Poll();
	Mode_Poll();
	Oled_Poll();
	//Cool_Mode_Poll();
	Running_Poll();
	Warn_Poll();
}



