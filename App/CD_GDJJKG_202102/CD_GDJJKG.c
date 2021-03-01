/**
  ******************************************************************************
  * @file    CD_GDJJKG.c 
  * @author  xxc
  * @version V1.0
  * @date    2021-02-19
  * @Company 
  * @brief    定时器输出38KHz频率，每次持续25ms
  *           外部中断捕捉下降沿，下降沿间隔180ms，累计3个以上
   ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */ 



/* Includes ------------------------------------------------------------------*/


#include "CD_GDJJKG.h" 


#define MODE1  0	//实时输出
#define MODE2  1	//
#define MODE3  2	//
#define MODE4  3	//


#define ENABLE_OUTPUT()    _pa6 = 1
#define DISABLE_OUTPUT()   _pa6 = 0
#define REVERSE_OUTPUT()   _pa6 = ~_pa6

#define ENABLE_OUTPUT_LED()    _pa7 = 1
#define DISABLE_OUTPUT_LED()   _pa7 = 0
#define REVERSE_OUTPUT_LED()   _pa7 = ~_pa7

#define OUTPUT_DELAYTIME    1220		//时基4.096ms，5000ms / 4.096 = 1220.7

uint8_t g_ucRunMode;

uint16_t	g_nCCRA ;		
uint16_t	g_nCCRP ;		
volatile uint8_t g_ucReviceCnt;
volatile uint16_t g_ucTimerBase0Cnt;
volatile uint8_t g_ucTimerBase1Cnt;
volatile uint16_t g_usOutPutDelay;

uint8_t g_ucSendStatus;
uint8_t g_ucReviceStatus;

uint8_t out_last;


/**
  * @brief  Configure GPIO
  * @param  None
  * @retval None
  */
void checkIR(void)
{
	uint8_t out;
	uint8_t edge;

	edge = 0;
	out = 0;
	if (g_ucReviceStatus)
	{
		g_ucReviceStatus = 0;
		if (g_ucTimerBase0Cnt > 180)
		{
			if (g_ucTimerBase0Cnt < 220)
			{
				out = 1;
			}
		}
		g_ucTimerBase0Cnt = 0;
	}


	if (out != out_last)
	{
		if (out)
		{
			edge = 1;	//上升沿，0->1
		}
		else
		{
			edge = 2;	//下降沿，1->0
		}
			
	}
	out_last = out;



	if (g_ucRunMode == MODE1)
	{			
		if (out)
		{
			ENABLE_OUTPUT();
		}
		else
		{
			DISABLE_OUTPUT();
		}
	}
	else if (g_ucRunMode == MODE2)
	{
		if (edge == 1)
		{
			REVERSE_OUTPUT();
		}
	}
	else if (g_ucRunMode == MODE3)
	{
		if (g_usOutPutDelay == 0)
		{
			DISABLE_OUTPUT();
			if (edge == 1)
			{
				ENABLE_OUTPUT();
				g_usOutPutDelay = OUTPUT_DELAYTIME;
			}
		}
	}
	else if (g_ucRunMode == MODE4)
	{
		if (g_usOutPutDelay == 0)
		{
			DISABLE_OUTPUT();
			if (edge == 2)
			{
				ENABLE_OUTPUT();
				g_usOutPutDelay = OUTPUT_DELAYTIME;
			}
		}
	}
}

/**
  * @brief  红外编码发射，180ms间隔发射持续25ms的38k方波
  * @param  None
  * @retval None
  */
void sendIR(void)
{
	if (g_ucSendStatus == 0)
	{
		if (g_ucTimerBase1Cnt > 43)	//等待180ms，180/4.096=43.9
		{
			STM_ENABLE();		//enable STM
			g_ucTimerBase1Cnt = 0;
			g_ucSendStatus = 1;
		}	
	}
	else
	{
		if (g_ucTimerBase1Cnt > 6)	//等待25ms，25/4.096=6.1
		{
			STM_DISABLE();
			g_ucTimerBase1Cnt = 0;
			g_ucSendStatus = 0;
		}
	}
}

/**
  * @brief  Configure GPIO
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
	_pac6 = 0;	//输出模式
	_pac7 = 0;

	_pac0 = 1;	//输入模式
	_pac1 = 1;
	_papu0 = 1;  //上拉
	_papu1 = 1; 
}

void InitApp(void)
{

	GPIO_Config();

	/* TB0:1.024ms, TB1:4.096ms */
	TimeBase_Init();

	/* enable TimeBase0 interrupt */
	TB0_ISR_ENABLE();
	TB1_ISR_ENABLE();

	/* enable TimeBase IP */
	TB_ENABLE();	

	/* external interrupt initialization, */
	EXIT_Init();
	
	INT_CLEAR_FLAG();		//clear external interrupt flag
	INT_ISR_ENABLE();		//enable external interrupt

	/* initialization STM IP */
	STM_Init();
	
	g_nCCRA = 128;	//PWM duty set 128
	g_nCCRP = 4;	//PWM period set 4*128
	
	/* Configure pwm function */
	STM_PwmOutputConfig();
	
	/* setting pwm duty and period 
	 * param1: CCRA value,pwm duty;
	 * param2: CCRP value,pwm period;
	 */
	STM_PwmUpdate(g_nCCRA,g_nCCRP);
	
	// STM_ENABLE();		//enable STM

	if (_pa0 && _pa0)
	{
		g_ucRunMode = MODE1;
	}
	else if (_pa0 && _pa1 == 0)
	{
		g_ucRunMode = MODE2;
	}
	else if (_pa0 == 0 && _pa1)
	{
		g_ucRunMode = MODE3;
	}	
	else if (_pa0 == 0 && _pa1 == 0)
	{
		g_ucRunMode = MODE4;
	}

	ENABLE_INT();			//enable global interrupt
	
	_delay(100);		//wait system stable
}



 
