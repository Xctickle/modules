

#include "modules.h"
#include "SoftwareTimer.h"


/* 这2个全局变量转用于 DelayMs() 函数 */
static volatile uint16_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* 定于软件定时器结构体变量 */
static SOFT_TMR s_tTmr[TMR_COUNT];

static void SoftTimerDec(SOFT_TMR *_tmr);
void SetTIM1forInt(void);



/*
*********************************************************************************************************
*	函 数 名: InitSoftwareTimer
*	功能说明: 配置硬件定时器中断，并初始化软件定时器变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitSoftwareTimer(void)
{
	uint8_t i = 0;

	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
	}

	/* 配置硬件定时器，周期1ms */
	// bsp_SetTIM4forInt(1);
	SetTIM1forInt();

}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartTimer
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*				_mode   : 工作模式，0单次模块，1自动模式
*	返 回 值: 无
*********************************************************************************************************
*/
void StartSoftwareTimer(uint8_t _id, uint16_t _period, TMR_MODE_E _mode)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		// MODULES_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT();  			/* 关中断 */

	s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = _mode;			/* 工作模式 */

	ENABLE_INT();  				/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StopTimer
*	功能说明: 停止一个定时器
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*	返 回 值: 无
*********************************************************************************************************
*/
void StopSoftwareTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		// MODULES_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT();  	/* 关中断 */

	s_tTmr[_id].Count = 0;				/* 实时计数器初值 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 自动工作模式 */

	ENABLE_INT();  		/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CheckTimer
*	功能说明: 检测定时器是否超时
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*	返 回 值: 返回 0 表示定时未到， 1表示定时到
*********************************************************************************************************
*/
uint8_t CheckSoftwareTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_SoftTimerDec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参:  _tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void SoftwareTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: DelayUs
*	功能说明: 延迟若干us.  不准确，未验证
*	形    参：延迟时间，单位US； 不大于2000us。
*	返 回 值: 无
*********************************************************************************************************
*/
void DelayUs(uint16_t _us)
{
	uint16_t i,a;

	for (i = 0; i < _us; i++)
	{
		// a = 2;
		// while (--a);
		;
	}
}

/*
*********************************************************************************************************
*	函 数 名: DelayMs
*	功能说明: ms级延迟，延迟精度为正负1ms
*	形    参:  n : 延迟长度，单位1 ms。 n 应大于2
*	返 回 值: 无
*********************************************************************************************************
*/
void DelayMs(uint16_t _ms)
{
	if (_ms == 0)
	{
		return;
	}
	else if (_ms == 1)
	{
		_ms = 2;
	}

	DISABLE_INT();  			/* 关中断 */

	s_uiDelayCount = _ms;
	s_ucTimeOutFlag = 0;

	ENABLE_INT();  				/* 开中断 */

	while (1)
	{
		// bsp_Idle();				/* CPU空闲执行的操作， 见 bsp.c 和 bsp.h 文件 */
		
		/*
			等待延迟时间到
			注意：编译器认为 s_ucTimeOutFlag = 0，所以可能优化错误，因此 s_ucTimeOutFlag 变量必须申明为 volatile
		*/
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SoftTimer_ISR
*	功能说明: 定时器中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SoftTimer_ISR(void)
{
	uint8_t i = 0;

	/* 每隔1ms进来1次 （仅用于 DelayMs） */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
		{
			s_ucTimeOutFlag = 1;
		}
	}
	WDTCON |= 0x10;

	/* 每隔1ms 进来一次 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		SoftwareTimerDec(&s_tTmr[i]);
	}
}


/*
*********************************************************************************************************
*	SC92F32X系列定时器配置
*********************************************************************************************************
*/
// void SetTIM1forInt(uint16_t _uiPeriod)
void SetTIM1forInt(void)
{
	TMCON = 0X07;    //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
	
	//T0设置
	TMOD |= 0x01;                 //0000 0001;Timer0设置工作方式1
	TL0 = (65536 - 24000)%256;    //溢出时间：时钟为Fsys，则24000*（1/Fsys）=1ms;
	TH0 = (65536 - 24000)/256;
	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0

}

void timer0() interrupt 1
{
    TL0 = (65536 - 24000)%256;
	TH0 = (65536 - 24000)/256;
	SoftTimer_ISR();
}

/*
*********************************************************************************************************
*	STM8S系列定时器配置
*********************************************************************************************************
*/
#if defined (STM8S105)

/*
*********************************************************************************************************
*	函 数 名: bsp_SetTIM2forInt
*	功能说明: 配置TIMn，用于简单的定时中断. 开启定时中断。 中断服务程序由应用程序实现。
*	形    参:_ulPeriod : 定时周期 （us）。 0 表示关闭。
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetTIM2forInt(uint32_t _ulPeriod)
{
	uint32_t uiCount = 0;

	if (_ulPeriod == 0)
	{
    	TIM2_Cmd(DISABLE);
		return;
	}

	/* TIM2是自动重载的16位定时器，计数方向为递增，当递增到0时可以产生定时器中断，并自动重装定时器初值 */
	/*	
		STM8 TIM configuration: 
		Count = ((Period * TIM counter clock) - 1) 
        Count = 65535 --> (65535 + 1) / TIM counter clock = max time
        Count = 1     --> (    1 + 1) / TIM counter clock = min time
	*/
	uiCount = ((_ulPeriod * (SYS_CLK / 16) / 1000000) - 1);
	if (uiCount > 65535)  /* 计算期望的计数时钟个数, 由于TIM2是16位的定时器，因此如果该值大于65535，则需要进行分频 */			
	{
		/* 打印出错的源代码文件名、函数名称 */
		MODULES_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}
	TIM2_DeInit();                  					/* 复位TIM2所有寄存器 */
	TIM2_ARRPreloadConfig(ENABLE);  					/* 预先装载使能 */
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, uiCount);		/* 设置预分频和定时器重载 */
	TIM2_ClearFlag(TIM2_FLAG_UPDATE); 					/* Clear TIM2 update flag */
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);  			/* 使能TIM2中断 */
	//TIM2_UpdateDisableConfig(ENABLE);					/* 使能TIM2自动溢出事件 */
	TIM2_Cmd(ENABLE);									/* 使能TIM2 */

}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetTIM4forInt
*	功能说明: 配置TIM4，用于简单的定时中断. 开启定时中断。 中断服务程序由应用程序实现。
*	形    参:_ulPeriod : 定时周期 （ms）。 0 表示关闭。
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetTIM4forInt(uint32_t _ulPeriod)
{
	uint16_t uiCount = 0;

	if (_ulPeriod == 0)
	{
    	TIM4_Cmd(DISABLE);
		return;
	}

	/* TIM4是自动重载的8位定时器，计数方向为递增，当递增到0时可以产生定时器中断，并自动重装定时器初值 */
	/*	
		STM8 TIM configuration: 
		Count = ((Period * TIM counter clock) - 1) 
        Count = 255 --> (255 + 1) / TIM counter clock = max time
        Count = 1   --> (  1 + 1) / TIM counter clock = min time
	*/
	uiCount = ((_ulPeriod * (SYS_CLK / 128) / 1000) - 1);
	if (uiCount > 255)  /* 计算期望的计数时钟个数, 由于TIM4是8位的定时器，因此如果该值大于255，则需要进行分频 */			
	{
		/* 打印出错的源代码文件名、函数名称 */
		MODULES_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}
	TIM4_DeInit();                  					/* 复位TIM4所有寄存器 */
	TIM4_ARRPreloadConfig(ENABLE);  					/* 预先装载使能 */
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, uiCount);		/* 设置预分频和定时器重载 */
	TIM4_ClearFlag(TIM4_FLAG_UPDATE); 					/* Clear TIM4 update flag */
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);  			/* 使能TIM4中断 */
	//TIM4_UpdateDisableConfig(ENABLE);					/* 使能TIM4自动溢出事件 */
	TIM4_Cmd(ENABLE);									/* 使能TIM4 */

}

#endif

/******************************************************************************************************/
