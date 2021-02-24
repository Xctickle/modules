



#include "ADC_Command.h"   




static struct ADC* head_handle = ((void *)0);
static struct ADC* exc_handle = ((void *)0);


void adc_init(struct ADC* handle, void(*ADC_Init_handler)())
{
    handle->ADC_Init_handler = ADC_Init_handler;
    handle->samp_count = ADC_SAMP_COUNT;
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
	for (i = 0; i < ADC_SAMP_COUNT; i++)
	{
		sum += handle->adc_value[i];
	}

	value = sum / ADC_SAMP_COUNT;	/* ADC采样值由若干次采样值平均 */
    
    return value;
}

void adc_loop(void)
{
    if (exc_handle)
    {
        if (exc_handle->samp_count == ADC_SAMP_COUNT)
        {
            exc_handle->ADC_Init_handler();
            ADC_STARTCONVERSION();
            exc_handle->samp_count--;
        }
        else if(exc_handle->samp_count > 0)
        {
            exc_handle->adc_value[ADC_SAMP_COUNT - 1 - exc_handle->samp_count] = ADC_GETCONVERSIONVALUE();
            ADC_STARTCONVERSION();
            exc_handle->samp_count--;
        }
        else
        {
            exc_handle->adc_value[ADC_SAMP_COUNT - 1 - exc_handle->samp_count] = ADC_GETCONVERSIONVALUE();
            exc_handle->samp_count = ADC_SAMP_COUNT;
            exc_handle = exc_handle->next;
        }
    }
    else
    {
        exc_handle = head_handle;
    }

}
