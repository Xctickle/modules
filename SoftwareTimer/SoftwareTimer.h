/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*
*********************************************************************************************************
*/

#ifndef __SOFTWARETIMER_H
#define __SOFTWARETIMER_H


/*
	在此定义若干个软件定时器全局变量
	注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
*/
#define TMR_COUNT	2			/* 软件定时器的个数 （定时器ID范围 0 - 1) */

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* 一次工作模式 */
	TMR_AUTO_MODE = 1		/* 自动定时工作模式 */
}TMR_MODE_E;

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint8_t Mode;		/* 计数器模式，1次性 */
	volatile uint8_t Flag;		/* 定时到达标志  */
	volatile uint16_t Count;	/* 计数器 */
	volatile uint16_t PreLoad;	/* 计数器预装值 */
}SOFT_TMR;

/* 提供给其他C文件调用的函数 */
void InitSoftwareTimer(void);
void StartSoftwareTimer(uint8_t _id, uint16_t _period, TMR_MODE_E _mode);
void StopSoftwareTimer(uint8_t _id);
uint8_t CheckSoftwareTimer(uint8_t _id);

void DelayMS(uint16_t _ms);
void DelayUs(uint16_t _us);

void SoftTimer_ISR(void);


#endif

/******************************************************************************************************/

