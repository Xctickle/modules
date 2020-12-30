/******************************************************************************
* @file    timer.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   
*
******************************************************************************
* @attention
*
*******************************************************************************/ 

#include "modules.h"
#include "Timer.h"   
#include "sysclk.h"   





/*
*********************************************************************************************************
*	SC92F32X系列定时器配置
*********************************************************************************************************
*/
#if defined (SC92F32X)
// void SetTIM1forInt(uint16_t _uiPeriod)
void SetTIM1forInt(void)
{
	TMCON = 0X07;    //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
	
	//T0设置
	TMOD |= 0x01;                 //0000 0001;Timer0设置工作方式1
	TL0 = (65536 - 24000)%256;    //溢出时间：时钟为Fsys，则24000*（1/Fsys）=1ms;
	TH0 = (65536 - 24000)/256;
	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0

}

void timer0() interrupt 1
{
    TL0 = (65536 - 24000)%256;
	TH0 = (65536 - 24000)/256;
	SoftTimer_ISR();
}

#endif

/*
*********************************************************************************************************
*	STM8S系列定时器配置
*********************************************************************************************************
*/
#if defined (STM8S105)

/*
*********************************************************************************************************
*	函 数 名: SetTIM2forInt
*	功能说明: 配置TIMn，用于简单的定时中断. 开启定时中断。 中断服务程序由应用程序实现。
*	形    参:_ulPeriod : 定时周期 （us）。 0 表示关闭。
*	返 回 值: 无
*********************************************************************************************************
*/
void SetTIM2forInt(uint32_t _ulPeriod)
{
	uint32_t uiCount = 0;

	if (_ulPeriod == 0)
	{
    	TIM2_Cmd(DISABLE);
		return;
	}

	/* TIM2是自动重载的16位定时器，计数方向为递增，当递增到0时可以产生定时器中断，并自动重装定时器初值 */
	/*	
		STM8 TIM configuration: 
		Count = ((Period * TIM counter clock) - 1) 
        Count = 65535 --> (65535 + 1) / TIM counter clock = max time
        Count = 1     --> (    1 + 1) / TIM counter clock = min time
	*/
	uiCount = ((_ulPeriod * (SYS_CLK / 16) / 1000000) - 1);
	if (uiCount > 65535)  /* 计算期望的计数时钟个数, 由于TIM2是16位的定时器，因此如果该值大于65535，则需要进行分频 */			
	{
		/* 打印出错的源代码文件名、函数名称 */
		//MODULES_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}
	TIM2_DeInit();                  					/* 复位TIM2所有寄存器 */
	TIM2_ARRPreloadConfig(ENABLE);  					/* 预先装载使能 */
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, uiCount);		/* 设置预分频和定时器重载 */
	TIM2_ClearFlag(TIM2_FLAG_UPDATE); 					/* Clear TIM2 update flag */
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);  			/* 使能TIM2中断 */
	//TIM2_UpdateDisableConfig(ENABLE);					/* 使能TIM2自动溢出事件 */
	TIM2_Cmd(ENABLE);									/* 使能TIM2 */

}

/*
*********************************************************************************************************
*	函 数 名: SetTIM4forInt
*	功能说明: 配置TIM4，用于简单的定时中断. 开启定时中断。 中断服务程序由应用程序实现。
*	形    参:_ulPeriod : 定时周期 （ms）。 0 表示关闭。
*	返 回 值: 无
*********************************************************************************************************
*/
void SetTIM4forInt(uint32_t _ulPeriod)
{
	uint16_t uiCount = 0;

	if (_ulPeriod == 0)
	{
    	TIM4_Cmd(DISABLE);
		return;
	}

	/* TIM4是自动重载的8位定时器，计数方向为递增，当递增到0时可以产生定时器中断，并自动重装定时器初值 */
	/*	
		STM8 TIM configuration: 
		Count = ((Period * TIM counter clock) - 1) 
        Count = 255 --> (255 + 1) / TIM counter clock = max time
        Count = 1   --> (  1 + 1) / TIM counter clock = min time
	*/
	uiCount = ((_ulPeriod * (SYS_CLK / 128) / 1000) - 1);
	if (uiCount > 255)  /* 计算期望的计数时钟个数, 由于TIM4是8位的定时器，因此如果该值大于255，则需要进行分频 */			
	{
		/* 打印出错的源代码文件名、函数名称 */
		//MODULES_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}
	TIM4_DeInit();                  					/* 复位TIM4所有寄存器 */
	TIM4_ARRPreloadConfig(ENABLE);  					/* 预先装载使能 */
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, uiCount);		/* 设置预分频和定时器重载 */
	TIM4_ClearFlag(TIM4_FLAG_UPDATE); 					/* Clear TIM4 update flag */
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);  			/* 使能TIM4中断 */
	//TIM4_UpdateDisableConfig(ENABLE);					/* 使能TIM4自动溢出事件 */
	TIM4_Cmd(ENABLE);									/* 使能TIM4 */

}

#endif

/******************************************************************************************************/

















