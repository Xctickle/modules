/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_timer.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*
*********************************************************************************************************
*/

#ifndef __SOFTWARETIMER_H
#define __SOFTWARETIMER_H


/*
	�ڴ˶������ɸ������ʱ��ȫ�ֱ���
	ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
*/
#define TMR_COUNT	2			/* �����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 1) */

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
}TMR_MODE_E;

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint16_t Count;	/* ������ */
	volatile uint16_t PreLoad;	/* ������Ԥװֵ */
}SOFT_TMR;

/* �ṩ������C�ļ����õĺ��� */
void InitSoftwareTimer(void);
void StartSoftwareTimer(uint8_t _id, uint16_t _period, TMR_MODE_E _mode);
void StopSoftwareTimer(uint8_t _id);
uint8_t CheckSoftwareTimer(uint8_t _id);

void DelayMS(uint16_t _ms);
void DelayUs(uint16_t _us);

void SoftTimer_ISR(void);


#endif

/******************************************************************************************************/

