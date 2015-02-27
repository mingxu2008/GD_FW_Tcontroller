#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f10x.h"

//输入寄存器起始地址
#define REG_INPUT_START       0x0000
//输入寄存器数量
#define REG_INPUT_NREGS       8
//保持寄存器起始地址
#define REG_HOLDING_START     0x0000
//保持寄存器数量
#define REG_HOLDING_NREGS     64

//线圈起始地址
#define REG_COILS_START       0x0000
//线圈数量
#define REG_COILS_SIZE        16

//开关寄存器起始地址
#define REG_DISCRETE_START    0x0000
//开关寄存器数量
#define REG_DISCRETE_SIZE     16

//保持寄存器内容
extern uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];
//保持寄存器起始地址
extern uint16_t usRegHoldingStart;


#endif
