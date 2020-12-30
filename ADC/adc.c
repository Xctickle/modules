



#include "adc.h"   




static struct ADC* head_handle = ((void *)0);
static struct ADC* exc_handle = ((void *)0);


void adc_init(struct ADC* handle, void(*ADC_Init_handler)())
{
    handle->ADC_Init_handler = ADC_Init_handler;
    handle->samp_count = SAMP_COUNT;
}

/**
  * @brief  Start the adc work, add the handle into work list.
  * @param  btn: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int adc_start(struct ADC* handle)
{
    struct ADC* target = head_handle;

    while(target)
    {
        if(target == handle) return -1;	//already exist.

        target = target->next;
    }

    handle->next = head_handle;
    head_handle = handle;
    return 0;
}

/**
  * @brief  Stop the adc work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void adc_stop(struct ADC* handle)
{
    struct ADC** curr;

    for(curr = &head_handle; *curr; )
    {
        struct ADC* entry = *curr;

        if (entry == handle)
        {
            *curr = entry->next;
        }
        else
            curr = &entry->next;
    }
}

uint16_t adc_getValue(struct ADC* handle)
{
    uint8_t i;
    uint16_t sum;
    uint16_t value;

	sum = 0;
	for (i = 0; i < SAMP_COUNT; i++)
	{
		sum += handle->adc_value[i];
	}

	value = sum / SAMP_COUNT;	/* ADC采样值由若干次采样值平均 */
    
    return value;
}

void adc_loop(void)
{
    // struct ADC* target;

    // for(target = head_handle; target; target = target->next)
    // {
    //     if (target->samp_count == SAMP_COUNT)
    //     {
    //         target->ADC_Init_handler();
    //         ADC1_StartConversion();
    //         target->samp_count--;
    //         break;
    //     }
    //     else if(target->samp_count > 0)
    //     {
    //         target->adc_value[SAMP_COUNT - 1 - target->samp_count] = ADC1_GetConversionValue();
    //         ADC1_StartConversion();
    //         target->samp_count--;
    //         break;
    //     }
    //     else
    //     {
    //         target->samp_count = SAMP_COUNT;
    //         // target = target->next;
    //     }
    // }

    if (exc_handle)
    {
        if (exc_handle->samp_count == SAMP_COUNT)
        {
            exc_handle->ADC_Init_handler();
            ADC1_StartConversion();
            exc_handle->samp_count--;
        }
        else if(exc_handle->samp_count > 0)
        {
            exc_handle->adc_value[SAMP_COUNT - 1 - exc_handle->samp_count] = ADC1_GetConversionValue();
            ADC1_StartConversion();
            exc_handle->samp_count--;
        }
        else
        {
            exc_handle->samp_count = SAMP_COUNT;
            exc_handle = exc_handle->next;
        }
    }
    else
    {
        exc_handle = head_handle;
    }

}


// void ADCPro(void)
// {
// 	static uint16_t buf[SAMP_COUNT];
// 	static uint8_t write;
// 	uint16_t sum;
// 	uint8_t i;

// #if defined (STM8S105)
// 	buf[write] = ADC1_GetConversionValue();
// #endif

// 	if (++write >= SAMP_COUNT)
// 	{
// 		write = 0;
// 	}

// 	/* 下面这段代码采用求平均值的方法进行滤波*/
// 	sum = 0;
// 	for (i = 0; i < SAMP_COUNT; i++)
// 	{
// 		sum += buf[i];
// 	}

// 	g_usAdcValue = sum / SAMP_COUNT;	/* ADC采样值由若干次采样值平均 */

// #if defined (STM8S105)
// 	ADC1_StartConversion();		/* 软件启动下次ADC转换 */
// #endif 

// }


// void InitADC(void)
// {

//     GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);            //输入，ADC



//     /* De-Init ADC peripheral*/
//     ADC1_DeInit();

//     /* Init ADC1 peripheral */
//     ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_0, ADC1_PRESSEL_FCPU_D2, \
//             ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0,\
//             DISABLE);

//     /* Enable EOC interrupt */
//     // ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);  


//     // /*Start Conversion */
//     // ADC1_StartConversion();
// }


// uint16_t getADCvalue(uint8_t _channel)
// {


// #if defined (STM8S105)
//     ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, _channel, ADC1_PRESSEL_FCPU_D2, \
//             ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, _channel,\
//             DISABLE);

//     ADC1_ClearFlag(ADC1_FLAG_EOC);     
//     ADC1_StartConversion();
//     while(!ADC1_GetFlagStatus(ADC1_FLAG_EOC));
//     return = ADC1_GetConversionValue();

// #endif

// }

