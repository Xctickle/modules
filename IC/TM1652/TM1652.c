/******************************************************************************
* @file    TM1652.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   
*
******************************************************************************
* @attention
*
*******************************************************************************/ 

#include "TM1652.h"   
#include "uart.h"   
#include "delay.h"   


#define BASE 10
#define TM1652_NUM 4

//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     --- .
//      D    DP
//  ��������                        0    1    2     3    4    5    6    7   8    9    -    |    F    NC   E
// const uint8_t digitToSegment[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x02,0x71,0x00,0x79};     
//  ��������                               0    1    2     3    4    5    6    7   8    9    a   b    c   d   E   F
static const uint8_t digitToSegment[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
static uint8_t minusSegments = 0x40;		//����


uint8_t TM1652_digits[TM1652_NUM] = {0x00};

extern void delay3ms(void);


void TM1652_showDigit(uint8_t pos, uint8_t seg)
{
	if (pos <= TM1652_NUM && pos > 0)
	{
    	TM1652_digits[pos - 1] = seg;
	}
}

void TM1652_showDots(uint8_t dots)
{
	if (dots <= TM1652_NUM && dots > 0)
	{
    	// TM1652_digits[dots] |= (dots & 0x80);
    	TM1652_digits[dots - 1] |= 0x80;
	}
}

void TM1652_showNumber(int16_t num)
{
	int8_t i;
	uint8_t negative;
	uint8_t digit;

    negative = FALSE;

	if (num < 0)
	{
		num = -num;
		negative = TRUE;
	}
	
	if (num == 0 && !TM1652_LEADING_ZERO) 
	{
		// Singular case - take care separately
		for(i = 0; i < TM1652_NUM; i++)
		{
			TM1652_digits[i] = 0;
		}
		TM1652_digits[TM1652_NUM - 1] = digitToSegment[0];
	}
	else 
	{
		for(i = TM1652_NUM - 1; i >= 0; --i)		//iΪ�޷�������ʱ�����µ�Ƭ��������ѭ��
		{
		    digit = num % BASE;

			if (digit == 0 && num == 0 && !TM1652_LEADING_ZERO)
			{
				// Leading zero is blank
				TM1652_digits[i] = 0;
			}
			else
			{
				TM1652_digits[i] = digitToSegment[digit & 0x0f];		//&0x0f ���θ�4λ
			}
				
			if (digit == 0 && num == 0 && negative) 
			{
				TM1652_digits[i] = minusSegments;
				negative = FALSE;
			}
			
			num /= BASE;
		}
    }
}


void TM1652_Init(void)
{
	TM1652_WRITE_INIT();
	TM1652_Clear();
}


void TM1652_Clear(void)
{
	uint8_t i = 0; 

	for(i = 0; i < TM1652_NUM; i++)
	{
		TM1652_digits[i] = 0x00; //��ʾ�������������ʾ����.  
	}
}


void TM1652_Display(void)
{
	uint8_t i = 0;  

	TM1652_WRITE_DATA(0x08);    //������ʾ��ַ�������1

	for(i = 0;i < TM1652_NUM; i++)
	{   
		TM1652_WRITE_DATA(TM1652_digits[i]);
	}
	P13 = 1;		 //TX��ʼ�ߵ�ƽ��
	delay3ms();
	delay3ms();
	TM1652_WRITE_DATA(0x18);    //������ʾ��ַ����
	TM1652_WRITE_DATA(0x12);    //8/16ռ�ձȡ�8/8����������8��5λ
	P13 = 1;		 //TX��ʼ�ߵ�ƽ��
}


















