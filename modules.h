

#ifndef __MODULES_H_
#define __MODULES_H_

/*
	stdlib.h : standard library��׼��ͷ�ļ�,������C��C++���Ե���õ����Ͷ����ϵͳ���������� malloc()��free()
	
	stdio.h : standard input & output ��׼������������⡣���ú����� printf()��getc()��putc()
	
	string.h : �����ַ�����ĺ��������ͷ�ļ��⣬���ú�����strlen��strcmp��strcpy
	
	" " �� < > ������:
	" " һ���ʾԴ�����Դ���h�ļ�������ʱ�������������ڹ�������ָ��������·���б���Ѱ��ͷ�ļ�
	< > һ���ʾ����ϵͳ�Դ���h�ļ�������ʱ��������������IDE�Դ��Ŀ��ļ���Ѱ��ͷ�ļ���
*/

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #include "sc92f732x_c.h"
#include "stm8s.h"



// #define C51
#define STM8S105

#if defined (C51)

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

#define ENABLE_INT()	EA = 1
#define DISABLE_INT()	EA = 0

#define NOP()			_nop_()					

#endif

#if defined (STM8S105)

#define ENABLE_INT()	enableInterrupts()		/* ʹ��ȫ���ж� */
#define DISABLE_INT()	disableInterrupts() 	/* ��ֹȫ���ж� */
#define FEED_WDG()		nop()					/* ι�� */
#define NOP()			nop()					

#endif


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
