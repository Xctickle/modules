#ifndef __MODULES_H_
#define __MODULES_H_


#include "stm8s.h"

#define CD_GYHLWMK

/* adc的宏 */
#define ADC_SAMP_COUNT 5
#define ADC_STARTCONVERSION()       ADC1_StartConversion()
#define ADC_GETCONVERSIONVALUE()    ADC1_GetConversionValue()


/* I2C的宏 */
#define GPIO_PORT_I2C_SCL	GPIOB			/* GPIO端口 */
#define GPIO_PORT_I2C_SDA	GPIOB			/* GPIO端口 */
#define I2C_SCL_PIN		GPIO_PIN_7			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_PIN_6			/* 连接到SDA数据线的GPIO */
#define I2C_SCL_1()  GPIO_PORT_I2C_SCL->ODR |= I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  GPIO_PORT_I2C_SCL->ODR &= (uint8_t)(~I2C_SCL_PIN) 	/* SCL = 0 */
#define I2C_SDA_1()  GPIO_PORT_I2C_SDA->ODR |= I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  GPIO_PORT_I2C_SDA->ODR &= (uint8_t)(~I2C_SDA_PIN) 	/* SDA = 0 */
#define I2C_SDA_READ()  ((GPIO_PORT_I2C_SDA->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#define I2C_SCL_READ()  ((GPIO_PORT_I2C_SCL->IDR & I2C_SCL_PIN) != 0)	/* 读SCL口线状态 */


#define ENABLE_INT()	enableInterrupts()		/* 使能全局中断 */
#define DISABLE_INT()	disableInterrupts() 	/* 禁止全局中断 */
#define FEED_WDG()		nop()					/* 喂狗 */
#define NOP()			nop()					


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
