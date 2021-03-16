

#ifndef __MODULES_H_
#define __MODULES_H_

/*
	stdlib.h : standard library标准库头文件,包含了C、C++语言的最常用的类型定义和系统函数，比如 malloc()、free()
	
	stdio.h : standard input & output 标准输入输出函数库。常用函数有 printf()、getc()、putc()
	
	string.h : 关于字符数组的函数定义的头文件库，常用函数有strlen、strcmp、strcpy
	
	" " 和 < > 的区别:
	" " 一般表示源代码自带的h文件。编译时，编译器优先在工程配置指定的搜索路径列表中寻找头文件
	< > 一般表示编译系统自带的h文件。编译时，编译器优先在IDE自带的库文件夹寻找头文件。
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
	#define TM1652_WRITE_DATA(d)   uartSendByte(d)  //串口19200波特率
	#define TM1652_WRITE_INIT()    uart0Init()
#endif





#define ENABLE_INT()	EA = 1
#define DISABLE_INT()	EA = 0
#define NOP()			_nop_()					


#if 0	/* 1表示使能调试语句， 0 表示不参与编译，不会影响编译结果 */
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
