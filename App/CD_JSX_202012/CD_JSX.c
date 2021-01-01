/**
  ******************************************************************************
  * @file    CD_JSX.c 
  * @author  xxc
  * @version V1.0
  * @date    2020-12-20
  * @Company 
  * @brief   Main program body
   ******************************************************************************
  * @attention
  * 
  * LCD
  * 123456789abcdefg
  * ◆ Humi1-max: 999
  * ◆ Humi1-min: 999
  * ◆ Humi2-max: 999
  * ◆ Humi2-min: 999

  * H1:99      T1:99
  * H2:99      T2:99  
  * Increase cnt:999
  * running...   
  
  ******************************************************************************
  */ 



/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "lcd12864.h"
#include "sysclk.h"
#include "timer.h"
#include "multi_button.h"
#include "multi_timer.h"
#include "adc.h"
#include "delay.h"
#include "sht3xRARP.h"





#define	RUN_LED_ON()	 	         (GPIO_WriteHigh(GPIOG, GPIO_PIN_1))
#define	RUN_LED_OFF()  	             (GPIO_WriteLow(GPIOG, GPIO_PIN_1))
#define	RUN_LED_REV()  	             (GPIO_WriteReverse(GPIOG, GPIO_PIN_1))



#define	RELAY1_ON()	 	                 (GPIO_WriteHigh(GPIOC, GPIO_PIN_7))
#define	RELAY1_OFF()  	                 (GPIO_WriteLow(GPIOC, GPIO_PIN_7))
#define	RELAY1_REV()  	                 (GPIO_WriteReverse(GPIOC, GPIO_PIN_7))

#define	RELAY2_ON()	 	                 (GPIO_WriteHigh(GPIOG, GPIO_PIN_0))
#define	RELAY2_OFF()  	                 (GPIO_WriteLow(GPIOG, GPIO_PIN_0))
#define	RELAY2_REV()  	                 (GPIO_WriteReverse(GPIOG, GPIO_PIN_0))


void Display_Manage(void);

extern void Delay_us(uint8_t t);
extern void Delay_ms(uint16_t t);
extern float get_Humi(uint16_t _id);


typedef enum 
{
    SET_HUMI1 = 0,
    SET_HUMI2,
    SET_HUMI3,  
    SET_HUMI4,      
    STANTBY, 
    RUNNING
} RunStatus;

typedef enum 
{
    DISPLAY_SET = 0,
    DISPLAY_STA
} DisplayStatus;

typedef enum 
{
    UNKNOWN = 0,
    DEC_HUMI,
    INC_HUMI
} ExcStatus;

RunStatus  Run_Status = STANTBY;  
DisplayStatus Display_index = DISPLAY_STA;
ExcStatus Exc_status = UNKNOWN;


char Display_str[4][16] = {0x20};

char Set_str1[16] = {"  Humi1-max:    "};
char Set_str2[16] = {"  Humi1-min:    "};
char Set_str3[16] = {"  Humi2-max:    "};
char Set_str4[16] = {"  Humi2-min:    "};

char Sta_str1[16] = {"H1:99RH   T1:99C"};
char Sta_str2[16] = {"H2:99RH   T2:99C"};
char Sta_str3[16] = {"                "};
char Sta_str4[16] = {"waiting......   "};
// char Sta_str4[16] = {"等待中。。。。。"};

char stantby_str[16] = {"未运行         "};
char wait_str[16] = {"等待中。。。。。"};
char dec_str[16] =  {"正在除湿。。。。"};
char inc_str4[16] = {"正在加湿。。。。"};



uint8_t  humi_set[4] = {55};
uint8_t  humi_sta[4] = {35};
__IO uint16_t Increase_cnt = 0;

Button btn_run;
Button btn_set;
Button btn_add;
Button btn_dec;

Timer softwatetimer1;
Timer softwatetimer2;

ADC adc_t0;
ADC adc_t1;
// ADC adc_t2;
// ADC adc_t3;
ADC adc_h0;
ADC adc_h1;
// ADC adc_h2;
// ADC adc_h3;





#define BLOCK_OPERATION    0    /* block 0 in data eeprom memory: address is 0x4000 */
// uint8_t GBuffer[6];


/* Private function prototypes -----------------------------------------------*/

static void GPIO_Config(void);

static void FLASH_Config(void);
void Flash_Write(void);
void Flash_Read(void);


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

    GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_IN_FL_NO_IT);            //输入，ADC

    // adc_init(&adc_t3,adc_channel0_init);
    // adc_init(&adc_h3,adc_channel1_init);
    // adc_init(&adc_t2,adc_channel2_init);
    // adc_init(&adc_h2,adc_channel3_init);
    adc_init(&adc_t1,adc_channel4_init);
    adc_init(&adc_h1,adc_channel5_init);
    adc_init(&adc_t0,adc_channel6_init);
    adc_init(&adc_h0,adc_channel7_init);

    // adc_start(&adc_t3);
    // adc_start(&adc_t2);
    adc_start(&adc_t1);
    adc_start(&adc_t0);
    // adc_start(&adc_h3);
    // adc_start(&adc_h2);
    adc_start(&adc_h1);
    adc_start(&adc_h0);
}


void softwatetimer1_callback()
{
    uint8_t i;
    uint16_t value;
    uint8_t exc_cnt = 0;

    value = adc_getValue(&adc_h0);
    humi_sta[0] = get_Humi(value);

    value = adc_getValue(&adc_t0);
    humi_sta[1] = get_Temp(value);

    value = adc_getValue(&adc_h1);
    humi_sta[2] = get_Humi(value);

    value = adc_getValue(&adc_t1);
    humi_sta[3] = get_Temp(value);


    if (Run_Status == RUNNING)
    {

        if(humi_set[0] && humi_set[1])
        {
            if (humi_sta[0] > humi_set[0])      //大于最大值
            {
                exc_cnt = 1;    ///除湿
                if(humi_set[2] && humi_set[3])
                {
                    if (humi_sta[2] < humi_set[2])      //小于最大值
                    {
                        exc_cnt = 0; 
                    }
                }
            }
            else if (humi_sta[0] < humi_set[1])     //小于最小值
            {
                exc_cnt = 2;    ///加湿
                if(humi_set[2] && humi_set[3])
                {
                    if (humi_sta[2] > humi_set[3])     //大于最小值
                    {
                        exc_cnt = 0;
                    }
                }
            }
        }


        if (exc_cnt == 2)
        {
            Exc_status = INC_HUMI;
            RELAY1_ON();
            RELAY2_OFF();
            // Increase_cnt++;
        }
        else if (exc_cnt == 1)
        {
            Exc_status = DEC_HUMI;
            RELAY1_OFF();
            RELAY2_ON();
        }
        else if (exc_cnt == 0)
        {
            Exc_status = UNKNOWN;
            RELAY1_OFF();
            RELAY2_OFF();
        }
        
    }
    else
    {
        Exc_status = UNKNOWN;
        RELAY1_OFF();
        RELAY2_OFF();
    }
    
    Display_Manage();

}

void softwatetimer2_callback()
{
    button_ticks();
    adc_loop();
}

uint8_t btn_run_GPIO(void) 
{
    if (GPIO_ReadInputPin(GPIOC, GPIO_PIN_3))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t btn_set_GPIO(void) 
{
    if (GPIO_ReadInputPin(GPIOE, GPIO_PIN_5))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t btn_add_GPIO(void) 
{
    if (GPIO_ReadInputPin(GPIOC, GPIO_PIN_1))
    {
        return 1;
    }
    else
    {
        return 0;
    }   
}

uint8_t btn_dec_GPIO(void) 
{
    if (GPIO_ReadInputPin(GPIOC, GPIO_PIN_2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void BTN_RUN_SINGLE_Click_Handler(void* btn)
{
    if (Display_index == DISPLAY_SET)
    {
        Display_index = DISPLAY_STA;
        Flash_Write();
    }
    
    if (Run_Status != RUNNING)
    {
        Run_Status = RUNNING;
    }
    else if (Run_Status == RUNNING)
    {
        Run_Status = STANTBY;  
    }   
}

void BTN_SET_SINGLE_Click_Handler(void* btn)
{
    // if (Run_Status < SET_HUMI4)
    if (Display_index == DISPLAY_SET)
    {
        Run_Status++;
        if (Run_Status == STANTBY)
        {
            Run_Status = SET_HUMI1;
        } 
    }
}

void BTN_SET_LONG_RRESS_START_Handler(void* btn)
{
    if (Display_index == DISPLAY_SET)
    {
        Display_index = DISPLAY_STA;
        Flash_Write();
    }
    else
    {
        Display_index = DISPLAY_SET;
        Run_Status = SET_HUMI1;
    }
}

void BTN_ADD_SINGLE_Click_Handler(void* btn)
{
    if (Display_index == DISPLAY_SET)
    {
        if (humi_set[Run_Status] < 99)
        {
            humi_set[Run_Status]++;
        }
    } 
}

void BTN_ADD_LONG_RRESS_START_Handler(void* btn)
{
    if (Display_index == DISPLAY_SET)
    {
        if (humi_set[Run_Status] < 95)
        {
            humi_set[Run_Status]+=5;
        }
    } 
}

void BTN_DEC_SINGLE_Click_Handler(void* btn)
{
    if (Display_index == DISPLAY_SET)
    {
        if (humi_set[Run_Status])
        {
            humi_set[Run_Status]--;
        }
    } 
}

void BTN_DEC_LONG_RRESS_START_Handler(void* btn)
{
    if (Display_index == DISPLAY_SET)
    {
        if (humi_set[Run_Status] > 5)
        {
            humi_set[Run_Status]-=5;
        }
    } 
}


void Flash_Write(void)
{
    uint8_t i;

    // for (i = 0; i < 4; i++)
    // {
    //     GBuffer[i] = humi_set[i];
    // }   

    for (i = 0; i < 4; i++)
    {
        FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS + i, humi_set[i]);
        Delay_us(250);
    }

}

void Flash_Read(void)
{
    uint8_t i;

    uint32_t add, startadd, stopadd;
    // uint32_t readcounter = 0;


    startadd = FLASH_DATA_START_PHYSICAL_ADDRESS + ((uint16_t)BLOCK_OPERATION * (uint16_t)FLASH_BLOCK_SIZE);
    // stopadd = startadd + (uint16_t)FLASH_BLOCK_SIZE;
    stopadd = startadd + (uint16_t)4;
    

    i = 0;
    for (add = startadd; add < stopadd; add++)
    {
        humi_set[i] = FLASH_ReadByte(add);
        i++;
    }
    
}

void Display_status(void)
{
    uint8_t humi1_set_char[2] = {0};
    uint8_t humi2_set_char[2] = {0};
    uint8_t humi3_set_char[2] = {0};
    uint8_t humi4_set_char[2] = {0};
    // uint8_t Increase_cnt_char[3] = {0};

    uint8_t hun, ten, one;

    uint8_t i, j;
    uint16_t temp;

    if(humi_sta[0] > 99)
    {
        humi1_set_char[0] = '-';
        humi1_set_char[1] = '-'; 
    }
    else
    {
        ten = humi_sta[0] / 10;
        one = humi_sta[0] % 10;
        humi1_set_char[0] = ten + 0x30;
        humi1_set_char[1] = one + 0x30;  
    }
    
    if(humi_sta[1] > 99)
    {
        humi2_set_char[0] = '-';
        humi2_set_char[1] = '-'; 
    }
    else
    {
        ten = humi_sta[1] / 10;
        one = humi_sta[1] % 10;
        humi2_set_char[0] = ten + 0x30;
        humi2_set_char[1] = one + 0x30;  
    }

    if(humi_sta[2] > 99)
    {
        humi3_set_char[0] = '-';
        humi3_set_char[1] = '-'; 
    }
    else
    {
        ten = humi_sta[2] / 10;
        one = humi_sta[2] % 10;
        humi3_set_char[0] = ten + 0x30;
        humi3_set_char[1] = one + 0x30;  
    }

    if(humi_sta[3] > 99)
    {
        humi4_set_char[0] = '-';
        humi4_set_char[1] = '-'; 
    }
    else
    {
        ten = humi_sta[3] / 10;
        one = humi_sta[3] % 10;
        humi4_set_char[0] = ten + 0x30;
        humi4_set_char[1] = one + 0x30;  
    }


    // hun = Increase_cnt / 100;
    // temp = Increase_cnt % 100;
    // ten = temp / 10;
    // one = temp % 10;

    // Increase_cnt_char[0] = hun + 0x30;
    // Increase_cnt_char[1] = ten + 0x30;
    // Increase_cnt_char[2] = one + 0x30;



    Sta_str1[3] = humi1_set_char[0];
    Sta_str1[4] = humi1_set_char[1];
    Sta_str1[13] = humi2_set_char[0];
    Sta_str1[14] = humi2_set_char[1];

    Sta_str2[3] = humi3_set_char[0];
    Sta_str2[4] = humi3_set_char[1];
    Sta_str2[13] = humi4_set_char[0];
    Sta_str2[14] = humi4_set_char[1];

    // Sta_str3[13] = Increase_cnt_char[0];
    // Sta_str3[14] = Increase_cnt_char[1];
    // Sta_str3[15] = Increase_cnt_char[2];



    if (Run_Status == RUNNING)
    {
        if (Exc_status == INC_HUMI)
        {
            for (i = 0; i < 16; i++)
            {
                Sta_str4[i] = inc_str4[i];
            }
        }
        else if (Exc_status == DEC_HUMI)
        {
            for (i = 0; i < 16; i++)
            {
                Sta_str4[i] = dec_str[i];
            }
        }
        else
        {
            for (i = 0; i < 16; i++)
            {
                Sta_str4[i] = wait_str[i];
            }
        }
    }
    else if (Run_Status == STANTBY)
    {
        for (i = 0; i < 16; i++)
        {
            Sta_str4[i] = stantby_str[i];
        }  
    }
    

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

void Display_Set(void)
{
    uint8_t humi1_set_char[2] = {0};
    uint8_t humi2_set_char[2] = {0};
    uint8_t humi3_set_char[2] = {0};
    uint8_t humi4_set_char[2] = {0};

    uint8_t ten, one;

    uint8_t i, j;


    ten = humi_set[0] / 10;
    one = humi_set[0] % 10;
    humi1_set_char[0] = ten + 0x30;
    humi1_set_char[1] = one + 0x30;  

    ten = humi_set[1] / 10;
    one = humi_set[1] % 10;
    humi2_set_char[0] = ten + 0x30;
    humi2_set_char[1] = one + 0x30;  

    ten = humi_set[2] / 10;
    one = humi_set[2] % 10;
    humi3_set_char[0] = ten + 0x30;
    humi3_set_char[1] = one + 0x30;  

    ten = humi_set[3] / 10;
    one = humi_set[3] % 10;
    humi4_set_char[0] = ten + 0x30;
    humi4_set_char[1] = one + 0x30;  


    Set_str1[14] = humi1_set_char[0];
    Set_str1[15] = humi1_set_char[1];
   
    Set_str2[14] = humi2_set_char[0];
    Set_str2[15] = humi2_set_char[1];

    Set_str3[14] = humi3_set_char[0];
    Set_str3[15] = humi3_set_char[1];

    Set_str4[14] = humi4_set_char[0];
    Set_str4[15] = humi4_set_char[1];


    switch(Run_Status)
    {
        case SET_HUMI1:
            Set_str1[0] = '>';
            Set_str2[0] = ' ';
            Set_str3[0] = ' ';
            Set_str4[0] = ' ';
        break;
        
        case SET_HUMI2:
            Set_str1[0] = ' ';
            Set_str2[0] = '>';
            Set_str3[0] = ' ';
            Set_str4[0] = ' ';
        break;

        case SET_HUMI3:
            Set_str1[0] = ' ';
            Set_str2[0] = ' ';
            Set_str3[0] = '>';
            Set_str4[0] = ' ';  
        break;

        case SET_HUMI4:
            Set_str1[0] = ' ';
            Set_str2[0] = ' ';
            Set_str3[0] = ' ';
            Set_str4[0] = '>';  
        break;
    }
    
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 16; j++)
        {
            switch(i)
            {
                case 0:
                    Display_str[i][j] = Set_str1[j];
                break;
                case 1:
                    Display_str[i][j] = Set_str2[j];
                break;
                case 2:
                    Display_str[i][j] = Set_str3[j];
                break;
                case 3:
                    Display_str[i][j] = Set_str4[j];
                break;
            }
        }
    }

}

void Display_Manage(void)
{

    if(Display_index == DISPLAY_STA)
    {
        if (Run_Status == RUNNING)
        {
            RUN_LED_ON();
        }
        else
        {
            RUN_LED_OFF();
        }
        Display_status();
    }
    else
    {
        RUN_LED_OFF();
        Display_Set();
    }

    LCD_PutString(0, 1, &Display_str[0][0], 16);
    LCD_PutString(0, 2, &Display_str[1][0], 16);
    LCD_PutString(0, 3, &Display_str[2][0], 16); 
    LCD_PutString(0, 4, &Display_str[3][0], 16); 
    
}

void Button_Config(void)
{

    GPIO_Init(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);


    button_init(&btn_run, btn_run_GPIO, 0);
	button_init(&btn_set, btn_set_GPIO, 0);
	button_init(&btn_add, btn_add_GPIO, 0);
	button_init(&btn_dec, btn_dec_GPIO, 0);
	

	button_attach(&btn_run, SINGLE_CLICK,     BTN_RUN_SINGLE_Click_Handler);

	button_attach(&btn_set, SINGLE_CLICK,     BTN_SET_SINGLE_Click_Handler);
	button_attach(&btn_set, LONG_RRESS_START, BTN_SET_LONG_RRESS_START_Handler);

	button_attach(&btn_add, SINGLE_CLICK,     BTN_ADD_SINGLE_Click_Handler);
	button_attach(&btn_add, LONG_PRESS_HOLD, BTN_ADD_LONG_RRESS_START_Handler);	

	button_attach(&btn_dec, SINGLE_CLICK,     BTN_DEC_SINGLE_Click_Handler);
	button_attach(&btn_dec, LONG_PRESS_HOLD, BTN_DEC_LONG_RRESS_START_Handler);

	button_start(&btn_run);
	button_start(&btn_set);
	button_start(&btn_add);
	button_start(&btn_dec);

}

void SoftwareTimer_Config(void)
{    

    timer_init(&softwatetimer1, softwatetimer1_callback, 200, 200); //1s loop
    timer_start(&softwatetimer1);

    timer_init(&softwatetimer2, softwatetimer2_callback, 20, 20); //5ms delay
    timer_start(&softwatetimer2);

}


void main(void)
{


    InitSysclk();
    SetTIM4forInt(1);


    GPIO_Config();

    FLASH_Config(); 
    
    SoftwareTimer_Config();

    adc_config();


    Button_Config();
    
    Flash_Read();

    Init_ST7920();
    ClrScreen();



    enableInterrupts();   

    Run_Status = STANTBY;


    while (1)
    {

        timer_loop();

    }
  
}


/**
  * @brief  Configure the FLASH for block programming
  * @param  None
  * @retval None
  */
void FLASH_Config(void)
{
 /* Define flash programming Time*/
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

  FLASH_Unlock(FLASH_MEMTYPE_PROG);
  /* Wait until Flash Program area unlocked flag is set*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
  {}

  /* Unlock flash data eeprom memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  /* Wait until Data EEPROM area unlocked flag is set*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
  {}
}


/**
  * @brief  Configure GPIO
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{


    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);      //relay1
    GPIO_Init(GPIOG, GPIO_PIN_0 | GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST);      //relay2，runled


}
 

