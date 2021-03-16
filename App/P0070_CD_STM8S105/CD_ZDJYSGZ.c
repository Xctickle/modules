/**
  ******************************************************************************
  * @file    CD_ZDJYSGZ.c 
  * @author  xxc
  * @version V1.0
  * @date    2021-1-5
  * @Company 
  * @brief   Main program body
   ******************************************************************************
  * @attention
  * 
  * LCD
  * 123456789abcdefg
  * H1:99      T1:99
  * H2:99      T2:99  
  * Increase cnt:999
  * running...   
  
  ******************************************************************************
  */ 



/* Includes ------------------------------------------------------------------*/


#include "CD_ZDJYSGZ.h"


void Display_Manage(void);


char Display_str[4][16] = {0x20};

char Sta_str1[16] = {" 1:          NTU"};
char Sta_str2[16] = {" 2:          NTU"};
char Sta_str3[16] = {" 3:          NTU"};
char Sta_str4[16] = {" 4:          NTU"};

Timer softwatetimer1;
Timer softwatetimer2;

ADC adc_t0;
ADC adc_t1;
ADC adc_t2;
ADC adc_t3;
// ADC adc_h0;
// ADC adc_h1;
// ADC adc_h2;
// ADC adc_h3;

uint16_t ntu[4];





void adc_channel0_init(void)
{
    ADC1_DeInit();

    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_0, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0,\
            DISABLE);
}

void adc_channel1_init(void)
{
    ADC1_DeInit();

    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_1, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL1,\
            DISABLE);
}

void adc_channel2_init(void)
{
    ADC1_DeInit();

    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL2,\
            DISABLE);
}

void adc_channel3_init(void)
{
    ADC1_DeInit();

    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3,\
            DISABLE);
}

void adc_channel4_init(void)
{
    ADC1_DeInit();

    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_4, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL4,\
            DISABLE);
}

void adc_channel5_init(void)
{
    ADC1_DeInit();

    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_5, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL5,\
            DISABLE);
}

void adc_channel6_init(void)
{
    ADC1_DeInit();

    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL6,\
            DISABLE);
}

void adc_channel7_init(void)
{
    ADC1_DeInit();

    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_7, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL7,\
            DISABLE);
}

void adc_config(void)
{

    GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_IN_FL_NO_IT);            // ‰»Î£¨ADC

    adc_init(&adc_t3,adc_channel0_init);
    // adc_init(&adc_h3,adc_channel1_init);
    adc_init(&adc_t2,adc_channel2_init);
    // adc_init(&adc_h2,adc_channel3_init);
    adc_init(&adc_t1,adc_channel4_init);
    // adc_init(&adc_h1,adc_channel5_init);
    adc_init(&adc_t0,adc_channel6_init);
    // adc_init(&adc_h0,adc_channel7_init);

    adc_start(&adc_t3);
    adc_start(&adc_t2);
    adc_start(&adc_t1);
    adc_start(&adc_t0);
    // adc_start(&adc_h3);
    // adc_start(&adc_h2);
    // adc_start(&adc_h1);
    // adc_start(&adc_h0);
}

uint16_t get_Ntu(uint16_t _adc)
{
    uint16_t value = 0;

    return value;
}


void softwatetimer1_callback()
{
    uint16_t value;


    value = adc_getValue(&adc_t0);
    ntu[0] = get_Ntu(value);

    value = adc_getValue(&adc_t1);
    ntu[1] = get_Ntu(value);

    value = adc_getValue(&adc_t2);
    ntu[2] = get_Ntu(value);

    value = adc_getValue(&adc_t3);
    ntu[3] = get_Ntu(value);

    
    Display_Manage();

}

void softwatetimer2_callback()
{
    adc_loop();
}



void NumToStr(int16_t _num, char str[], uint8_t _cnt)
{
	uint8_t i;
	uint8_t sta = 0;

    for (i = 0; i < _cnt; i++)
    {
        str[_cnt - i] = _num%10+'0';
        _num /= 10;
        if (sta == 0)
        {
            if (str[_cnt - i] == '0')
            {
                str[_cnt - i] = ' ';
            }
            else
            {
                sta = 1;
            }
        } 
    }
} 


void Display_status(void)
{
    char tempstr[5] = {0};

    uint8_t i, j;

    
    NumToStr(ntu[0],tempstr,5);
    Sta_str1[4] = tempstr[0];
    Sta_str1[5] = tempstr[1];
    Sta_str1[6] = tempstr[0];
    Sta_str1[7] = tempstr[1];
    Sta_str1[8] = tempstr[1];

    NumToStr(ntu[1],tempstr,5);
    Sta_str2[4] = tempstr[0];
    Sta_str2[5] = tempstr[1];
    Sta_str2[6] = tempstr[0];
    Sta_str2[7] = tempstr[1];
    Sta_str2[8] = tempstr[1];

    NumToStr(ntu[2],tempstr,5);
    Sta_str3[4] = tempstr[0];
    Sta_str3[5] = tempstr[1];
    Sta_str3[6] = tempstr[0];
    Sta_str3[7] = tempstr[1];
    Sta_str3[8] = tempstr[1];

    NumToStr(ntu[3],tempstr,5);
    Sta_str4[4] = tempstr[0];
    Sta_str4[5] = tempstr[1];
    Sta_str4[6] = tempstr[0];
    Sta_str4[7] = tempstr[1];
    Sta_str4[8] = tempstr[1];


    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 16; j++)
        {
            switch(i)
            {
                case 0:
                    Display_str[i][j] = Sta_str1[j];
                break;
                case 1:
                    Display_str[i][j] = Sta_str2[j];
                break;
                case 2:
                    Display_str[i][j] = Sta_str3[j];
                break;
                case 3:
                    Display_str[i][j] = Sta_str4[j];
                break;
            }
        }
    }

}

void Display_Manage(void)
{

    Display_status();
    LCD_PutString(0, 1, &Display_str[0][0], 16);
    LCD_PutString(0, 2, &Display_str[1][0], 16);
    LCD_PutString(0, 3, &Display_str[2][0], 16); 
    LCD_PutString(0, 4, &Display_str[3][0], 16); 
    
}



void SoftwareTimer_Config(void)
{    

    timer_init(&softwatetimer1, softwatetimer1_callback, 200, 200); //1s loop
    timer_start(&softwatetimer1);

    timer_init(&softwatetimer2, softwatetimer2_callback, 20, 20); //5ms delay
    timer_start(&softwatetimer2);

}


void InitApp(void)
{

    InitSysclk();
    SetTIM4forInt(1);

    SoftwareTimer_Config();

    adc_config();

    Init_ST7920();
    ClrScreen();

    enableInterrupts();   

}


