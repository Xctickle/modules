#ifndef __MODULES_H_
#define __MODULES_H_


#include "HT68F0025.h"


#define CD_GDJJKG

#define ENABLE_INT()	_emi = 1		/* ʹ��ȫ���ж� */
#define DISABLE_INT()	_emi = 0		/* ��ֹȫ���ж� */
#define FEED_WDG()		GCC_CLRWDT()	/* ι�� */

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
