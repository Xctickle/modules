

#ifndef __MODULES_H_
#define __MODULES_H_




#include "stm8s.h"

#include "lcd12864.h"
#include "sysclk.h"
#include "timer.h"
#include "multi_timer.h"
#include "adc.h"
#include "delay.h"

#define	CS_HIGH	 	 (GPIO_WriteHigh(GPIOC, GPIO_PIN_6))
#define	CS_LOW  	 (GPIO_WriteLow(GPIOC, GPIO_PIN_6))
#define	CS_REV  	 (GPIO_WriteReverse(GPIOC, GPIO_PIN_6))

#define	SID_HIGH	 (GPIO_WriteHigh(GPIOC, GPIO_PIN_5))
#define	SID_LOW  	 (GPIO_WriteLow(GPIOC, GPIO_PIN_5))
#define	SID_REV  	 (GPIO_WriteReverse(GPIOC, GPIO_PIN_5))

#define	CLK_HIGH	 (GPIO_WriteHigh(GPIOC, GPIO_PIN_4))
#define	CLK_LOW  	 (GPIO_WriteLow(GPIOC, GPIO_PIN_4))
#define	CLK_REV  	 (GPIO_WriteReverse(GPIOC, GPIO_PIN_4))


#define	SET_CS_OUT       (GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_OUT_PP_HIGH_FAST))
#define	SET_SID_OUT      (GPIO_Init(GPIOC,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_FAST))
#define	SET_CLK_OUT      (GPIO_Init(GPIOC,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_FAST))


#define ENABLE_INT()	enableInterrupts()		/* ʹ��ȫ���ж� */
#define DISABLE_INT()	disableInterrupts() 	/* ��ֹȫ���ж� */
#define FEED_WDG()		nop()					/* ι�� */
#define NOP()			nop()					


#if 0	/* 1��ʾʹ�ܵ�����䣬 0 ��ʾ��������룬����Ӱ������� */
	#define MODULES_Printf		printf
#else
	#define MODULES_Printf    (...)
#endif

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif


#endif

/******************************************************************************************************/
