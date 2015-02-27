#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f10x.h"

//����Ĵ�����ʼ��ַ
#define REG_INPUT_START       0x0000
//����Ĵ�������
#define REG_INPUT_NREGS       8
//���ּĴ�����ʼ��ַ
#define REG_HOLDING_START     0x0000
//���ּĴ�������
#define REG_HOLDING_NREGS     64

//��Ȧ��ʼ��ַ
#define REG_COILS_START       0x0000
//��Ȧ����
#define REG_COILS_SIZE        16

//���ؼĴ�����ʼ��ַ
#define REG_DISCRETE_START    0x0000
//���ؼĴ�������
#define REG_DISCRETE_SIZE     16

//���ּĴ�������
extern uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];
//���ּĴ�����ʼ��ַ
extern uint16_t usRegHoldingStart;


#endif
