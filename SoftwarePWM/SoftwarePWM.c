

#include "softwarePWM.h"


//SoftPWM _handle list head.
static struct SoftPWM* head_handle = 0;



/*
*********************************************************************************************************
*	函 数 名: bsp_InitPWM
*	功能说明: 初始化PWM模块
*	形    参: _uiCycle : 周期。 0 表示关闭。
*	形    参: _ucDuty : 占空比，（0-100）
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSoftPWM(SoftPWM* _handle, void(*timeout_cb)(), uint8_t Polarity)
{
    _handle->timeout_cb = timeout_cb;
    _handle->Polarity = 1;
    _handle->Status = 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitPWM
*	功能说明: 初始化PWM模块
*	形    参: _uiCycle : 周期，单位us。 0 表示关闭。
*	形    参: _ucDuty : 占空比，（0-100）
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetSoftPWM(SoftPWM* _handle, uint32_t _cycle, uint32_t _duty)
{
    uint32_t cc1, cc2;
    // cc2 = _cycle / SOFTPWM_TIMEBASE;
    // cc1 = _duty / 100.0 * cc2;

    cc2 = _cycle;
    cc1 = _duty;


    _handle->CC1 = cc1;
    _handle->CC2 = cc2 - cc1;
    _handle->Timeout = cc1;

    // _handle->Timeout = _SoftPWM_ticks + cc1;

}

/**
  * @brief  Start the SoftPWM work, add the _handle into work list.
  * @param  btn: target _handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int bsp_SoftPWM_start(SoftPWM* _handle)
{
    struct SoftPWM* target = head_handle;

    while(target)
    {
        if(target == _handle) return -1;	//already exist.

        target = target->Next;
    }

    _handle->Next = head_handle;
    head_handle = _handle;

    return 0;
}

/**
  * @brief  Stop the SoftPWM work, remove the _handle off work list.
  * @param  _handle: target _handle strcut.
  * @retval None
  */
void bsp_SoftPWM_stop(SoftPWM* _handle)
{
    struct SoftPWM** curr;

    for(curr = &head_handle; *curr; )
    {
        struct SoftPWM* entry = *curr;

        if (entry == _handle)
        {
            *curr = entry->Next;
        }
        else
        {
            curr = &entry->Next;
        }
    }
}


/*
*********************************************************************************************************
*	函 数 名: bsp_SoftPWM_ISR
*	功能说明: 定时器中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SoftwarePWM_ISR(void)
{
    struct SoftPWM* target;

    for(target = head_handle; target; target = target->Next)
    {
        target->Ticks++;
        if(target->Ticks >= target->Timeout)
        {
            if (target->Status)
            {
                target->Timeout = target->CC2;
            }
            else
            {
                target->Timeout = target->CC1;
            }
            target->Ticks = 0;
            target->Status = ~target->Status;
            target->timeout_cb();
        }
    }
}

/*
*********************************************************************************************************
*
*	STM8S系列定时器配置
*
*********************************************************************************************************
*/
#if defined (STM8S105)

/*
*********************************************************************************************************
*	函 数 名: bsp_SetTIM1forPWM
*	功能说明: 配置TIMn，用于简单的定时中断. 开启定时中断。 中断服务程序由应用程序实现。
*	形    参:_ulPeriod : 定时周期 （us）。 0 表示关闭。
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetTIM1forPWM(void)
{

   TIM1_DeInit();

    /* TIM1 configuration:
    - TIM1CLK is set to 16 MHz, the TIM1 Prescaler is equal to 128 so the TIM1 counter
    clock used is 16 MHz / 64 = 250 000 Hz
    - With 125 000 Hz we can generate time base:
        max time base is 2.048 ms if TIM1_PERIOD = 255 --> (255 + 1) / 250000 = 2.048 ms
        min time base is 0.016 ms if TIM1_PERIOD = 1   --> (  1 + 1) / 250000 = 0.016 ms
    - In this example we need to generate a time base equal to 20 ms
    so TIM1_PERIOD = (0.02 * 250000 - 1) = 4999 */


	TIM1_TimeBaseInit(64, TIM1_COUNTERMODE_UP, 4999, 0);

	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	
	/*
	TIM1_OCMode = TIM1_OCMODE_PWM2
	TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
	TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
	TIM1_Pulse = CCR1_Val
	TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
	TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
	TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
	TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
	
	*/

		/* 	(0.0050 * 250000 - 1) = 124		0.5ms	0度
			(0.0010 * 250000 - 1) = 249		1ms		45度
			(0.0015 * 250000 - 1) = 374		1.5ms	90度
			(0.0020 * 250000 - 1) = 499		2ms		135度
			(0.0025 * 250000 - 1) = 624		2.5ms	180度 
			TIM1_SetCompare1(624);
		*/

	TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
				624, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
				TIM1_OCNIDLESTATE_RESET); 


	/* TIM1 counter enable */
	TIM1_Cmd(ENABLE);

	/* TIM1 Main Output Enable */
	TIM1_CtrlPWMOutputs(ENABLE);
}

#endif


/******************************************************************************************************/
