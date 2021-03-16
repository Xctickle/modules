#ifndef __MODULES_H_
#define __MODULES_H_


#include "HT8_MCU_IP_SEL.h"


#define CD_GDJJKG

#define ENABLE_INT()	_emi = 1		/* ʹ��ȫ���ж� */
#define DISABLE_INT()	_emi = 0		/* ��ֹȫ���ж� */
#define FEED_WDG()		_clrwdt()	/* ι�� */


#define NOP()			nop()					

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;


#endif
