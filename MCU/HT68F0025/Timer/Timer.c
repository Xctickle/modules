/******************************************************************************
* @file    timer.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   ʹ��STC���߼��㣬1T��ʱ��
*
******************************************************************************
* @attention
*
*******************************************************************************/ 

#include "Timer.h"   
#include "multi_timer.h"

/* ϵͳʱ��Ƶ�� */
#define SYS_CLK	 24000000u   	//24M


/**
  * @brief  1����@24.000MHz
  * @param  None
  * @retval None
  */
void timer0SetforInt(void)		//��ʱ1ms
{
	/*
	TMOD |= 0x01;                 //0000 0001;Timer0���ù�����ʽ1��16λ��ʱ��
	TL0 = (65536 - 24000)%256;    //���ʱ�䣺ʱ��ΪFsys����24000*��1/Fsys��=1ms; 
	TH0 = (65536 - 24000)/256;
	max��65536 * ��1/SYS_CLK�� = 0.00273s = 2.73ms
	min��1 * ��1/SYS_CLK�� = 0.0000416666ms
	1ms��0.001s / ��1/SYS_CLK�� = 24000
	*/
	TMCON |= 0x01;    	//------001 ;Timer0ѡ��ʱ��Fsys // TMCON &= ~0x01;     //------000 ;Timer0ѡ��ʱ��Fsys/12
	TMOD |= 0x01;       //0000 0001;Timer0���ù�����ʽ1��16λ��ʱ��
	TL0 = 0x40;			//1����@24.000MHz
	TH0 = 0xA2;
	TR0 = 0;			//��ʱ��0����
	ET0 = 1;			//��ʱ��0�ж�
	TR0 = 1;			//��ʱ��0ʹ��
}

/**
  * @brief  timer0 interrupt.
  * @param  None
  * @retval None
  */
void timer0() interrupt 1
{
	TL0 = 0x40;		//1����@24.000MHz
	TH0 = 0xA2;
	timer_ticks();
}











