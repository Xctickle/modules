/******************************************************************************
* @file    delay.c
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
#include "delay.h"   
#include "sysclk.h"   





void Delay_us(uint8_t t)
{
    uint8_t i = 16;
	while(--t)
	{
    	while(i--);
	}
}

void Delay_ms(uint16_t ms)
{
	uint16_t i;
	while(--ms)
	{
		i = 1000;
		while(--i) 
			Delay_us(1);
	}
}













