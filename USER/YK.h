#ifndef __YK_H
#define __YK_H
#define TxBufferSize2   64
#define RxBufferSize2   64

#define YK_SUCCESS 0x89
#define YK_FAIL 0XE0

#define AIR_CMD_TYPE 0X02
#define AIR_CMD_ONOFF 0X04
#define AIR_CMD_MODE 0X05
#define AIR_CMD_TEMP 0X06
#define AIR_CMD_WS 0X07
#define AIR_CMD_WD 0X08


extern u8  TxBuffer2[TxBufferSize2];
extern u8  RxBuffer2[RxBufferSize2];
extern __IO u8 TxCounter2;
extern __IO u8 RxCounter2;
extern u8 NumofTX;
extern u8 NumofRX;
extern void Air_Poll(void);
extern void Air_Init(void);

#endif
