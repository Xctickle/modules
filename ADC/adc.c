



#include "adc.h"   

void InitADC(void)
{

    /* De-Init ADC peripheral*/
    ADC1_DeInit();

    /* Init ADC1 peripheral */
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_0, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0,\
            DISABLE);

    /* Enable EOC interrupt */
    ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);  


    // /*Start Conversion */
    // ADC1_StartConversion();
}


uint16_t getADCvalue(uint8_t _channel)
{


#if defined (STM8S105)
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, _channel, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, _channel,\
            DISABLE);


    ADC1_ClearITPendingBit(ADC1_IT_EOC);     
    ADC1_StartConversion();
    while(!ADC1_GetITStatus(ADC1_IT_EOC));
    return = ADC1_GetConversionValue();

#endif


}