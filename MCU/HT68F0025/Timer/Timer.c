/******************************************************************************
* @file    timer.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   使用STC工具计算，1T定时器
*
******************************************************************************
* @attention
*
*******************************************************************************/ 

#include "Timer.h"   
#include "multi_timer.h"

/* 系统时钟频率 */
#define SYS_CLK	 24000000u   	//24M


/**
  * @brief  1毫秒@24.000MHz
  * @param  None
  * @retval None
  */
void timer0SetforInt(void)		//定时1ms
{
	/*
	TMOD |= 0x01;                 //0000 0001;Timer0设置工作方式1，16位定时器
	TL0 = (65536 - 24000)%256;    //溢出时间：时钟为Fsys，则24000*（1/Fsys）=1ms; 
	TH0 = (65536 - 24000)/256;
	max：65536 * （1/SYS_CLK） = 0.00273s = 2.73ms
	min：1 * （1/SYS_CLK） = 0.0000416666ms
	1ms：0.001s / （1/SYS_CLK） = 24000
	*/
	TMCON |= 0x01;    	//------001 ;Timer0选择时钟Fsys // TMCON &= ~0x01;     //------000 ;Timer0选择时钟Fsys/12
	TMOD |= 0x01;       //0000 0001;Timer0设置工作方式1，16位定时器
	TL0 = 0x40;			//1毫秒@24.000MHz
	TH0 = 0xA2;
	TR0 = 0;			//定时器0除能
	ET0 = 1;			//定时器0中断
	TR0 = 1;			//定时器0使能
}

/**
  * @brief  timer0 interrupt.
  * @param  None
  * @retval None
  */
void timer0() interrupt 1
{
	TL0 = 0x40;		//1毫秒@24.000MHz
	TH0 = 0xA2;
	timer_ticks();
}











