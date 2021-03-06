#ifndef __MODULES_H_
#define __MODULES_H_


#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tft_lcd.h"
#include "LCD_ILI9486.h"
#include "SoftwareI2C.h"
#include "delay.h"
#include "sys.h"


#define CD_ZKYHWMK



/* STM32_I2C�� */
#define I2C_STM32
#define I2C_DELAY     		200				/* I2C����λ�ӳ٣����400KHz */
#define RCC_I2C_PORT 		RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define GPIO_PORT_I2C_SCL	GPIOB			/* GPIO�˿� */
#define GPIO_PORT_I2C_SDA	GPIOB			/* GPIO�˿� */
#define I2C_SCL_PIN			GPIO_Pin_10			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN			GPIO_Pin_11			/* ���ӵ�SDA�����ߵ�GPIO */
#define I2C_SCL_1()  		GPIO_PORT_I2C_SCL->BSRR = I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  		GPIO_PORT_I2C_SCL->BRR = I2C_SCL_PIN				/* SCL = 0 */
#define I2C_SDA_1()  		GPIO_PORT_I2C_SDA->BSRR = I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  		GPIO_PORT_I2C_SDA->BRR = I2C_SDA_PIN				/* SDA = 0 */
#define I2C_SDA_READ()  	((GPIO_PORT_I2C_SDA->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#define I2C_SCL_READ()  	((GPIO_PORT_I2C_SCL->IDR & I2C_SCL_PIN) != 0)	/* ��SCL����״̬ */



#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
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

