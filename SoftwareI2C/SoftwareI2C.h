#ifndef _SOFTWAREI2C_H_
#define _SOFTWAREI2C_H_



#include "modules.h"

// /* STM32_I2C宏 */
// #define I2C_STM32
// #define I2C_DELAY           200				            /* uint16 I2C总线位延迟，最快400KHz */
// #define RCC_I2C_PORT 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
// #define GPIO_PORT_I2C_SCL	GPIOB			            /* GPIO端口 */
// #define GPIO_PORT_I2C_SDA	GPIOB			            /* GPIO端口 */
// #define I2C_SCL_PIN		    GPIO_Pin_10			        /* 连接到SCL时钟线的GPIO */
// #define I2C_SDA_PIN		    GPIO_Pin_11			        /* 连接到SDA数据线的GPIO */
// #define I2C_SCL_1()         GPIO_PORT_I2C_SCL->BSRR = I2C_SCL_PIN				/* SCL = 1 */
// #define I2C_SCL_0()         GPIO_PORT_I2C_SCL->BRR = I2C_SCL_PIN				/* SCL = 0 */
// #define I2C_SDA_1()         GPIO_PORT_I2C_SDA->BSRR = I2C_SDA_PIN				/* SDA = 1 */
// #define I2C_SDA_0()         GPIO_PORT_I2C_SDA->BRR = I2C_SDA_PIN				/* SDA = 0 */
// #define I2C_SDA_READ()      ((GPIO_PORT_I2C_SDA->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
// #define I2C_SCL_READ()      ((GPIO_PORT_I2C_SCL->IDR & I2C_SCL_PIN) != 0)	/* 读SCL口线状态 */
// #define I2C_IO_INIT()       NOP()
// #define I2C_SCL_IN()        NOP()
// #define I2C_SCL_OUT()       NOP()
// #define I2C_SDA_IN()        NOP()
// #define I2C_SDA_OUT()       NOP()

// /* STM8S_I2C宏 */
// #define I2C_STM8S
// #define I2C_DELAY           200				            /* uint16 I2C总线位延迟，最快400KHz */
// #define GPIO_PORT_I2C_SCL	GPIOB			            /* GPIO端口 */
// #define GPIO_PORT_I2C_SDA	GPIOB			            /* GPIO端口 */
// #define I2C_SCL_PIN		    GPIO_PIN_7			        /* 连接到SCL时钟线的GPIO */
// #define I2C_SDA_PIN		    GPIO_PIN_6			        /* 连接到SDA数据线的GPIO */
// #define I2C_SCL_1()         GPIO_PORT_I2C_SCL->ODR |= I2C_SCL_PIN				/* SCL = 1 */
// #define I2C_SCL_0()         GPIO_PORT_I2C_SCL->ODR &= (uint8_t)(~I2C_SCL_PIN) 	/* SCL = 0 */
// #define I2C_SDA_1()         GPIO_PORT_I2C_SDA->ODR |= I2C_SDA_PIN				/* SDA = 1 */
// #define I2C_SDA_0()         GPIO_PORT_I2C_SDA->ODR &= (uint8_t)(~I2C_SDA_PIN) 	/* SDA = 0 */
// #define I2C_SDA_READ()      ((GPIO_PORT_I2C_SDA->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
// #define I2C_SCL_READ()      ((GPIO_PORT_I2C_SCL->IDR & I2C_SCL_PIN) != 0)	/* 读SCL口线状态 */
// #define I2C_IO_INIT()       NOP()
// #define I2C_SCL_IN()        NOP()
// #define I2C_SCL_OUT()       NOP()
// #define I2C_SDA_IN()        NOP()
// #define I2C_SDA_OUT()       NOP()

// /* SC92F_I2C宏 */
// #define I2C_SC92F
// #define I2C_DELAY     200				            /* uint16 I2C总线位延迟，最快400KHz */
// #define I2C_SCL_PIN		P17							/* 连接到SCL时钟线的GPIO */
// #define I2C_SDA_PIN		P16							/* 连接到SDA数据线的GPIO */
// #define I2C_SCL_1()  I2C_SCL_PIN = 1				/* SCL = 1 */
// #define I2C_SCL_0()  I2C_SCL_PIN = 0			 	/* SCL = 0 */
// #define I2C_SDA_1()  I2C_SDA_PIN = 1				/* SDA = 1 */
// #define I2C_SDA_0()  I2C_SDA_PIN = 0			 	/* SDA = 0 */
// #define I2C_SDA_READ()  I2C_SDA_PIN					/* 读SDA口线状态 */
// #define I2C_SCL_READ()  I2C_SCL_PIN					/* 读SCL口线状态 */
// #define I2C_IO_INIT()    P1PH &= 0xC0               /* 1100 0000 p17、p16取消上拉电阻 */
// #define I2C_SCL_IN()    P1CON &= ~0x80              /* 输入模式 */
// #define I2C_SCL_OUT()   P1CON |= 0x80               /* 输出模式 */
// #define I2C_SDA_IN()    P1CON &= ~0x40
// #define I2C_SDA_OUT()   P1CON |= 0x40


#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

void InitI2C(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
uint8_t I2CBusBusy(void);

#endif
