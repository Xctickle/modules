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



/* STM32_I2C宏 */
#define I2C_STM32
#define I2C_DELAY     		200				/* I2C总线位延迟，最快400KHz */
#define RCC_I2C_PORT 		RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define GPIO_PORT_I2C_SCL	GPIOB			/* GPIO端口 */
#define GPIO_PORT_I2C_SDA	GPIOB			/* GPIO端口 */
#define I2C_SCL_PIN			GPIO_Pin_10			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN			GPIO_Pin_11			/* 连接到SDA数据线的GPIO */
#define I2C_SCL_1()  		GPIO_PORT_I2C_SCL->BSRR = I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  		GPIO_PORT_I2C_SCL->BRR = I2C_SCL_PIN				/* SCL = 0 */
#define I2C_SDA_1()  		GPIO_PORT_I2C_SDA->BSRR = I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  		GPIO_PORT_I2C_SDA->BRR = I2C_SDA_PIN				/* SDA = 0 */
#define I2C_SDA_READ()  	((GPIO_PORT_I2C_SDA->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#define I2C_SCL_READ()  	((GPIO_PORT_I2C_SCL->IDR & I2C_SCL_PIN) != 0)	/* 读SCL口线状态 */



#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
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

