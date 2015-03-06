#ifndef __T_APP_H
#define __T_APP_H
#include "main.h"

#define ON 1
#define OFF 0
#define MODE_NUM 7
#define D10S 10000
#define D500MS 500

extern uint16_t  *T_REG;

enum T_Flag{
	UNSTART,
	START,
	FINISH,	
};


enum T_Mode{
	COOL_S,
	COOL_E,
	COOL_H,
	DFROST,
	COMP_CLOSE,
	T_OFF,
	AUTO_CTR,
	
};

enum T_INDEX{
	COMPRESSOR,				//compressor
	DEFROST,					//defrost register
	LIGHT,		//LIGHT
	FAN,			//FAN
	DEMIST, 	//DEMIST
	AUX,			//auxiliary
	DEFROST_T1,
	DEFROST_T2,
	DEFROST_T3,
	DEFROST_T4,
	DEFROST_T5,
	DEFROST_T6,
	DEFROST_DURA,
	DEFROST_TEMP,
	COMP_DELAY,
	SETPOINT,
	TEMP_1,		//COOLING
	TEMP_2,		//DEFROST
	TEMP_OFFSET1,
	TEMP_OFFSET2,
	TEMP_DIFF,
	WARNING,
	WARN_TEMP,
	WARN_DELAY,
	RUNNING_MODE,
	COOL_MODE,
	T_RTC1,
	T_RTC2,
	T_RTC_CFG1,
	T_RTC_CFG2,
	T_RTC_CFG,
		};

extern uint32_t Button_t,Mode_t;
extern uint8_t button_flag;
extern uint16_t oled_t;	
extern uint16_t warn_i;
extern uint8_t warn_flag;
extern uint16_t defrost_i;
extern __IO uint8_t defrost_flag;
extern __IO uint16_t comp_i;
extern __IO uint8_t comp_flag;
extern void T_Init(void);
//extern void Reg_Poll(void);
//extern void Relay_Poll(void);
extern void T_Poll(void);
		

#endif

