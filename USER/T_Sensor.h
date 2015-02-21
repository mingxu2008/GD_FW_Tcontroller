#ifndef  __T_SENSOR_H
#define  __T_SENSOR_H
#include "stm32f10x.h"
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define HOLD_REG_TEMP 0X00

extern	void ADC1_Init(void);
extern void ADC1_Poll(uint16_t *adc_tmp);

#endif
