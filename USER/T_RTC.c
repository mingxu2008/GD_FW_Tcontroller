#include "T_RTC.h"
#include "stm32f10x.h"
#include "T_App.h"

__IO uint32_t TimeDisplay = 0;

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Adjusts time.
  * @param  None
  * @retval None
  */
void Time_Adjust(void)
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter(0x1517f);			//23:59:59
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

void T_RTC_Init(void)
{
	NVIC_Configuration();
	  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */


    /* RTC Configuration */
    RTC_Configuration();



    /* Adjust time by values entered by the user on the hyperterminal */
//   Time_Adjust();
		RTC_WaitForLastTask();
    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		PWR_BackupAccessCmd(ENABLE);
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      //printf("\r\n\n Power On Reset occurred....");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
    //  printf("\r\n\n External Reset occurred....");
    }

   // printf("\r\n No need to configure RTC....");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
		RTC_WaitForLastTask();
    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }

}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}


/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Toggle LED1 */
    //STM_EVAL_LEDToggle(LED1);

    /* Enable time update */
    TimeDisplay = 1;

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    
		/*warn delay*/
		if(warn_i < T_REG[WARN_DELAY])
		{
			warn_i ++;
		}
		else
		{
			warn_flag = 1;
			warn_i = 0;
		}
#if 0
		/*defrost delay*/
		if(defrost_i < T_REG[DEFROST_DURA])
		{
			defrost_i ++;
		}
		else
		{
		//	defrost_flag = FINISH;
		}
#endif
		/*COMPRESSOR delay*/
		if(comp_i < usRegHoldingBuf[COMP_DELAY])
		{
			comp_i ++;
		}
		else
		{
			comp_flag = 1;
			comp_i = 0;
		}		
  }
}

void T_RTC_Poll(void)
{
	uint32_t tmp_RTC;
	if(T_REG[T_RTC_CFG] == ON)				//set RTC Value
	{
		tmp_RTC = (T_REG[T_RTC_CFG1]<<16) | (T_REG[T_RTC_CFG2]);
		if(tmp_RTC <= 0x0001517F)		//max 23:59:59
		{
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
			
			RTC_SetCounter(tmp_RTC);
     /* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
		}
		T_REG[T_RTC_CFG1] = 0;
		T_REG[T_RTC_CFG2] = 0;
		T_REG[T_RTC_CFG] = OFF;
	}
	if(TimeDisplay == 1)									//read RTC Value
	{
		TimeDisplay = 0;
		tmp_RTC = RTC_GetCounter();
		if(tmp_RTC == 0x0001517F)			//23:59:59
		{
			RTC_SetCounter(0x0);
     /* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
		}
		T_REG[T_RTC2] = (uint16_t)(tmp_RTC);
		T_REG[T_RTC1] = (uint16_t)(tmp_RTC>>16);
		
	}

}	

