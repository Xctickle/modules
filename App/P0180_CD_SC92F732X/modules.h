

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



#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
 #include<intrins.h>			//nop

#include "sc92f732x_c.h"

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

// /*!< Signed integer types  */
// typedef   signed char     int8_t;
// typedef   signed int      int16_t;
// typedef   signed long     int32_t;

// /*!< Unsigned integer types  */
// typedef   unsigned char     uint8_t;
// typedef   unsigned int      uint16_t;
// typedef   unsigned long     uint32_t;

#include "softwareI2C.h"
#include "uart.h"   
#include "Delay.h"

#include "sht3xDIS.h"
// #include "sht3x.h"
// #include "sgp30.h"
// #include "sensirion_arch_config.h"
// #include "sensirion_i2c.h"






/* C51_I2C�� */
#define I2C_C51
#define I2C_DELAY     200				/* I2C����λ�ӳ٣����400KHz */

#define I2C_SCL_PIN		P17			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		P16			/* ���ӵ�SDA�����ߵ�GPIO */
#define I2C_SCL_IN()    P1CON &= ~0x80
#define I2C_SCL_OUT()   P1CON |= 0x80
#define I2C_SDA_IN()    P1CON &= ~0x40
#define I2C_SDA_OUT()   P1CON |= 0x40

#define I2C_SCL_1()  I2C_SCL_PIN = 1				/* SCL = 1 */
#define I2C_SCL_0()  I2C_SCL_PIN = 0			 	/* SCL = 0 */
#define I2C_SDA_1()  I2C_SDA_PIN = 1				/* SDA = 1 */
#define I2C_SDA_0()  I2C_SDA_PIN = 0			 	/* SDA = 0 */

#define I2C_SDA_READ()  I2C_SDA_PIN					/* ��SDA����״̬ */
#define I2C_SCL_READ()  I2C_SCL_PIN					/* ��SCL����״̬ */

#define IC_DS18B20
#ifdef IC_DS18B20
	#define	DQ2_SET() 	 		P20 = 1
	#define	DQ2_CLR()	 		P20 = 0
	#define	DQ2_RED()  	 		P20
	#define DQ2_IN()            P2CON &= ~0x01
	#define DQ2_OUT()           P2CON |= 0x01
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
