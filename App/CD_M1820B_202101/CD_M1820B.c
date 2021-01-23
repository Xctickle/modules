/*
*********************************************************************************************************
*
*	ģ������ : ������
*	�ļ����� : main.h
*	��    �� : V1.0
*	˵    �� : M1820B\DS18B20�����¶ȴ�������ʾ����
*			1������ͨ�ţ���ȡ�¶�
*			2��TM1629B���������
*			3����������
*			3��

*	�޸ļ�¼ :
*		�汾��   ����       ����    ˵��
*		v1.0   2020-12-15  xxc  
*
*
*********************************************************************************************************
*/


#include "CD_M1820B.h"
#include "multi_timer.h"
#include "DS18B20.h"
#include "TM1652.h"
#include "Timer.h"


Timer softwatetimer1;
float temperature;
int16_t tp;


extern void timer0SetforInt(void);

extern float DS18B20_GetSingelTemp(void);
extern void DS18B20_SingleConvert(void);

extern void TM1652_Init(void);
extern void TM1652_Display(void);
extern void TM1652_showNumber(int16_t num);
extern void TM1652_showDots(uint8_t dots);
extern void TM1652_showDigit(uint8_t pos, uint8_t seg);


void softwatetimer1_callback()
{

    temperature = DS18B20_GetSingelTemp();
	if (temperature > -30)
	{
		DS18B20_SingleConvert();
		tp = temperature * 10;
		if (tp > 999)		//��������
		{
			tp = 999;
		}
		// tp = 9876;
		TM1652_showNumber(tp);
		TM1652_showDots(3);
	}
	else
	{
		TM1652_showDigit(1, 0x40);		// -
		TM1652_showDigit(2, 0x40);		// -
		TM1652_showDigit(3, 0x40);		// -
		TM1652_showDigit(4, 0x40);		// -
	}
	
	TM1652_Display();
}



void SoftwareTimer_Config(void)
{    
    timer_init(&softwatetimer1, softwatetimer1_callback, 1000, 1000); //200ms loop
    timer_start(&softwatetimer1);

}

void GPIO_Config(void)
{    
	P0CON = 0x08;		//p03���
	P2CON = 0x03;		//p20��p21���

	P2CON |= 0x00;		//p22��p23����
	P2PH |= 0x0C;		//p22��p23��������


	SC92F7321_NIO_Init();
}

void InitApp(void)
{

    GPIO_Config();

    timer0SetforInt();

	TM1652_Init();
    SoftwareTimer_Config();


    ENABLE_INT();   

}

