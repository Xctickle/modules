/**
  ******************************************************************************
  * @file    sysclk.c 
  * @author  xxc
  * @version V1.0
  * @date    2018-2-3
  * @brief   
   ******************************************************************************
  * @attention
  *  
  ******************************************************************************
  */ 

#include "modules.h"
#include "sysclk.h"


/*
*********************************************************************************************************
*	函 数 名: InitSysclk
*	功能说明: 配置系统时钟
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitSysclk(void)
{




/*
*********************************************************************************************************
*
*	STM8S系列时钟配置
*
*********************************************************************************************************
*/
#if defined (STM8S105)

    //内部高速RC振荡源16M, 分频倍率设置为 1/1 可得到16M
    /* 自动切换, 切换到内部高速时钟HSI, 禁止系统时钟中断, 关闭当前的时钟源  */
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  /* 设置内部高速RC时钟HSI为时钟源，分频系数为1，16M */
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* 等待HSI时钟稳定 */

#endif



}


/******************************************************************************************************/
