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

#include "Timer.h"   



/**
  * @brief  timer2��ʱ������
  * @param  _ulPeriod us��ʱ����
  * @retval None
  */
void timer2SetforInt(uint32_t _ulPeriod)
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

/**
  * @brief  timer4��ʱ������
  * @param  _ulPeriod ms��ʱ����
  * @retval None
  */
void timer4SetforInt(uint8_t _ucPeriod)
{
	uint16_t uiCount = 0;

	if (_ucPeriod == 0)
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
	uiCount = ((_ucPeriod * (SYS_CLK / 128) / 1000) - 1);
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


















