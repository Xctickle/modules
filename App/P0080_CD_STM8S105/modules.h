#ifndef __MODULES_H_
#define __MODULES_H_


#include "stm8s.h"

#define CD_GYHLWMK

/* adc�ĺ� */
#define ADC_SAMP_COUNT 5
#define ADC_STARTCONVERSION()       ADC1_StartConversion()
#define ADC_GETCONVERSIONVALUE()    ADC1_GetConversionValue()


/* I2C�ĺ� */
#define GPIO_PORT_I2C_SCL	GPIOB			/* GPIO�˿� */
#define GPIO_PORT_I2C_SDA	GPIOB			/* GPIO�˿� */
#define I2C_SCL_PIN		GPIO_PIN_7			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_PIN_6			/* ���ӵ�SDA�����ߵ�GPIO */
#define I2C_SCL_1()  GPIO_PORT_I2C_SCL->ODR |= I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  GPIO_PORT_I2C_SCL->ODR &= (uint8_t)(~I2C_SCL_PIN) 	/* SCL = 0 */
#define I2C_SDA_1()  GPIO_PORT_I2C_SDA->ODR |= I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  GPIO_PORT_I2C_SDA->ODR &= (uint8_t)(~I2C_SDA_PIN) 	/* SDA = 0 */
#define I2C_SDA_READ()  ((GPIO_PORT_I2C_SDA->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#define I2C_SCL_READ()  ((GPIO_PORT_I2C_SCL->IDR & I2C_SCL_PIN) != 0)	/* ��SCL����״̬ */


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
