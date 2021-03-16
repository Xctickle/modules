

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

#include "sc92f732x_c.h"

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

#define IC_DS18B20
#ifdef IC_DS18B20
	#define	DQ2_SET() 	 		P20 = 1
	#define	DQ2_CLR()	 		P20 = 0
	#define	DQ2_RED()  	 		P20
	#define DQ2_IN()            P2CON &= ~0x01
	#define DQ2_OUT()           P2CON |= 0x01
#endif

#define IC_TM1652
#ifdef IC_TM1652
	#define TM1652_LEADING_ZERO 	FALSE
	#define TM1652_WRITE_DATA(d)   uartSendByte(d)  //����19200������
	#define TM1652_WRITE_INIT()    uart0Init()
#endif





#define ENABLE_INT()	EA = 1
#define DISABLE_INT()	EA = 0
#define NOP()			_nop_()					


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
