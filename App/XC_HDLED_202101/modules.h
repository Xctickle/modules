#ifndef __MODULES_H_
#define __MODULES_H_


#include "stm8s.h"

#define CD_HDLED


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
