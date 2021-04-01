#ifndef __MODULES_H_
#define __MODULES_H_


#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LCD_ILI9486.h"
#include "tft_lcd.h"
#include "SoftwareI2C.h"
#include "delay.h"
#include "sys.h"
#include "bsp_uart_fifo.h"
#include "c_misc.h"

#define P0120_CD



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

/* STM32_���ں� */
#define	UART1_FIFO_EN	1
#define	UART2_FIFO_EN	0
#define	UART3_FIFO_EN	0
#define	UART4_FIFO_EN	0
#define	UART5_FIFO_EN	0
#define RS485_EN		0
/* RS485оƬ����ʹ��GPIO, PB2 */
#define RCC_RS485_TXEN 	 RCC_APB2Periph_GPIOB
#define PORT_RS485_TXEN  GPIOB
#define PIN_RS485_TXEN	 GPIO_Pin_2
#define RS485_RX_EN()	PORT_RS485_TXEN->BRR = PIN_RS485_TXEN
#define RS485_TX_EN()	PORT_RS485_TXEN->BSRR = PIN_RS485_TXEN
/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			9600
	#define UART1_TX_BUF_SIZE	1*1024
	#define UART1_RX_BUF_SIZE	1*1024
#endif
#if UART2_FIFO_EN == 1
	#define UART2_BAUD			115200
	#define UART2_TX_BUF_SIZE	1*1024
	#define UART2_RX_BUF_SIZE	1*1024
#endif
#if UART3_FIFO_EN == 1
	#define UART3_BAUD			9600
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif
#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif
#if UART5_FIFO_EN == 1
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif









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

