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
*	SC92F32Xϵ�ж�ʱ������
*********************************************************************************************************
*/
#if defined (SC92F32X)
// void SetTIM1forInt(uint16_t _uiPeriod)
void SetTIM1forInt(void)
{
	TMCON = 0X07;    //------111 ;Timer0��Tiemr1��Tiemr2ѡ��ʱ��Fsys
	
	//T0����
	TMOD |= 0x01;                 //0000 0001;Timer0���ù�����ʽ1
	TL0 = (65536 - 24000)%256;    //���ʱ�䣺ʱ��ΪFsys����24000*��1/Fsys��=1ms;
	TH0 = (65536 - 24000)/256;
	TR0 = 0;
	ET0 = 1;//��ʱ��0����
	TR0 = 1;//�򿪶�ʱ��0

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
*	STM8Sϵ�ж�ʱ������
*********************************************************************************************************
*/
#if defined (STM8S105)

/*
*********************************************************************************************************
*	�� �� ��: SetTIM2forInt
*	����˵��: ����TIMn�����ڼ򵥵Ķ�ʱ�ж�. ������ʱ�жϡ� �жϷ��������Ӧ�ó���ʵ�֡�
*	��    ��:_ulPeriod : ��ʱ���� ��us���� 0 ��ʾ�رա�
*	�� �� ֵ: ��
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

	/* TIM2���Զ����ص�16λ��ʱ������������Ϊ��������������0ʱ���Բ�����ʱ���жϣ����Զ���װ��ʱ����ֵ */
	/*	
		STM8 TIM configuration: 
		Count = ((Period * TIM counter clock) - 1) 
        Count = 65535 --> (65535 + 1) / TIM counter clock = max time
        Count = 1     --> (    1 + 1) / TIM counter clock = min time
	*/
	uiCount = ((_ulPeriod * (SYS_CLK / 16) / 1000000) - 1);
	if (uiCount > 65535)  /* ���������ļ���ʱ�Ӹ���, ����TIM2��16λ�Ķ�ʱ������������ֵ����65535������Ҫ���з�Ƶ */			
	{
		/* ��ӡ�����Դ�����ļ������������� */
		//MODULES_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}
	TIM2_DeInit();                  					/* ��λTIM2���мĴ��� */
	TIM2_ARRPreloadConfig(ENABLE);  					/* Ԥ��װ��ʹ�� */
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, uiCount);		/* ����Ԥ��Ƶ�Ͷ�ʱ������ */
	TIM2_ClearFlag(TIM2_FLAG_UPDATE); 					/* Clear TIM2 update flag */
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);  			/* ʹ��TIM2�ж� */
	//TIM2_UpdateDisableConfig(ENABLE);					/* ʹ��TIM2�Զ�����¼� */
	TIM2_Cmd(ENABLE);									/* ʹ��TIM2 */

}

/*
*********************************************************************************************************
*	�� �� ��: SetTIM4forInt
*	����˵��: ����TIM4�����ڼ򵥵Ķ�ʱ�ж�. ������ʱ�жϡ� �жϷ��������Ӧ�ó���ʵ�֡�
*	��    ��:_ulPeriod : ��ʱ���� ��ms���� 0 ��ʾ�رա�
*	�� �� ֵ: ��
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

	/* TIM4���Զ����ص�8λ��ʱ������������Ϊ��������������0ʱ���Բ�����ʱ���жϣ����Զ���װ��ʱ����ֵ */
	/*	
		STM8 TIM configuration: 
		Count = ((Period * TIM counter clock) - 1) 
        Count = 255 --> (255 + 1) / TIM counter clock = max time
        Count = 1   --> (  1 + 1) / TIM counter clock = min time
	*/
	uiCount = ((_ulPeriod * (SYS_CLK / 128) / 1000) - 1);
	if (uiCount > 255)  /* ���������ļ���ʱ�Ӹ���, ����TIM4��8λ�Ķ�ʱ������������ֵ����255������Ҫ���з�Ƶ */			
	{
		/* ��ӡ�����Դ�����ļ������������� */
		//MODULES_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}
	TIM4_DeInit();                  					/* ��λTIM4���мĴ��� */
	TIM4_ARRPreloadConfig(ENABLE);  					/* Ԥ��װ��ʹ�� */
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, uiCount);		/* ����Ԥ��Ƶ�Ͷ�ʱ������ */
	TIM4_ClearFlag(TIM4_FLAG_UPDATE); 					/* Clear TIM4 update flag */
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);  			/* ʹ��TIM4�ж� */
	//TIM4_UpdateDisableConfig(ENABLE);					/* ʹ��TIM4�Զ�����¼� */
	TIM4_Cmd(ENABLE);									/* ʹ��TIM4 */

}

#endif

/******************************************************************************************************/

















