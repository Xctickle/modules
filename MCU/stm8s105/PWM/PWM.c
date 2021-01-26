/******************************************************************************
* @file    timer.c
* @author  xxc
* @version V1.0
* @date    2021-1-15
* @brief   
*
******************************************************************************
* @attention
*
*******************************************************************************/ 

#include "PWM.h"




#ifdef CD_HDLED
/**
  * @brief  timer1 PWM���ã�20ms���ڣ�XC_HEDLED��Ŀʹ��
  * 		ͨ��1���������
  * 		ͨ��2��LED����
  * @param  _c1 :�����ʼλ��
  * 		_c2 :LED��ʼ����
  * @retval None
  */
void timer1SetforPWM(uint16_t _c1, uint16_t _c2)
{

   TIM1_DeInit();

    /* TIM1 configuration:
    - TIM1CLK is set to 16 MHz, the TIM1 Prescaler is equal to 128 so the TIM1 counter
    clock used is 16 MHz / 64 = 250 000 Hz
    - With 125 000 Hz we can generate time base:
        max time base is 2.048 ms if TIM1_PERIOD = 255 --> (255 + 1) / 250000 = 2.048 ms
        min time base is 0.016 ms if TIM1_PERIOD = 1   --> (  1 + 1) / 250000 = 0.016 ms
    - In this example we need to generate a time base equal to 20 ms
    so TIM1_PERIOD = (0.02 * 250000 - 1) = 4999 */


	TIM1_TimeBaseInit(64, TIM1_COUNTERMODE_UP, 4999, 0);

	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	
	/*
	TIM1_OCMode = TIM1_OCMODE_PWM2
	TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
	TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
	TIM1_Pulse = CCR1_Val
	TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
	TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
	TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
	TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
	
	*/

	/* 	(0.0050 * 250000 - 1) = 124		0.5ms	0��
		(0.0010 * 250000 - 1) = 249		1ms		45��
		(0.0015 * 250000 - 1) = 374		1.5ms	90��
		(0.0020 * 250000 - 1) = 499		2ms		135��
		(0.0025 * 250000 - 1) = 624		2.5ms	180�� 
		TIM1_SetCompare1(624);
	*/
	TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
				_c1, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
				TIM1_OCNIDLESTATE_RESET); 
				
	TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
				_c2, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
				TIM1_OCNIDLESTATE_RESET); 

	/* TIM1 counter enable */
	TIM1_Cmd(ENABLE);

	/* TIM1 Main Output Enable */
	TIM1_CtrlPWMOutputs(ENABLE);
}
#endif
