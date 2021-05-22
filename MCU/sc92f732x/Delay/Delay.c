/******************************************************************************
* @file    delay.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   STC工具计算，STC-Y5指令
*
******************************************************************************
* @attention
*
*******************************************************************************/ 

#include "delay.h"   


void delay500us()		//@24.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}


void delay68us()		//@24.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 147;
	do
	{
		while (--j);
	} while (--i);
}


void delay48us()		//@24.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 27;
	do
	{
		while (--j);
	} while (--i);
}

void delay1us()		//@24.000MHz
{
	unsigned char i;

	;
	;
	i = 3;
	while (--i);
}


void delay1ms(void)		//@24.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

void delay3ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 71;
	j = 6;
	do
	{
		while (--j);
	} while (--i);
}


void Delay_ms(uint16_t ms)
{
	uint16_t i,j;
	while(--ms)
	{
		i = 1000;
		while(--i) 
		{
			j = 3;
			while (--j);
		}
			
	}
}






