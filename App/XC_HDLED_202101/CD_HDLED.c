/**
  ******************************************************************************
  * @file    CD_HDLED.c 
  * @author  xxc
  * @version V1.0
  * @date    2021-1-7
  * @Company 
  * @brief   Main program body
   ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */ 



/* Includes ------------------------------------------------------------------*/

#include "CD_HDLED.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CLOSING 0
#define OPENING 1

/* Private variables ---------------------------------------------------------*/
Timer swtimer1;
Timer swtimer2;
Button btn_run;

Severs_T g_tSeverPetal;
BreathLED_T g_tBreathLED;
BreathLED_T g_tWS2812B;

uint8_t g_ucPetalStatus = CLOSING;


/* Private function prototypes -----------------------------------------------*/
uint8_t btn_run_GPIO(void);
void swtimer1_callback(void);
void swtimer2_callback(void);
void BTN_RUN_SINGLE_Click_Handler(void* btn);

void Button_Config(void);
void SoftwareTimer_Config(void);
void BreathLED_Config(void);
void BreathLED_Command(void);
void SeverPetal_Config(void);
void SeverPetal_Command(void);
void WS2812B_Config(void);
void WS2812B_Command(void);

void GPIO_Config(void);



void swtimer1_callback(void)
{
    SeverPetal_Command();
    BreathLED_Command();
    WS2812B_Command();
}

void swtimer2_callback(void)
{
    button_ticks();
}

uint8_t btn_run_GPIO(void) 
{
    if (GPIO_ReadInputPin(GPIOB, GPIO_PIN_1)){return 1;}else{return 0;}
}


void BTN_RUN_SINGLE_Click_Handler(void* btn)
{
    g_ucPetalStatus = !g_ucPetalStatus;
    if (g_ucPetalStatus == OPENING)
    {
        g_tSeverPetal.targetAngle = PETAL_OPEN_ANGLE;
        g_tBreathLED.targetBrightness = LED_OPEN_BRIGHTNESS;
        g_tWS2812B.targetBrightness = WS2812B_OPEN_BRIGHTNESS;

    }
    else
    {
        g_tSeverPetal.targetAngle = PETAL_CLOSE_ANGLE;
        g_tBreathLED.targetBrightness = LED_CLOSE_BRIGHTNESS;
        g_tWS2812B.targetBrightness = WS2812B_CLOSE_BRIGHTNESS;
    }
}


void Button_Config(void)
{
    // GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);
    button_init(&btn_run, btn_run_GPIO, 1);
	button_attach(&btn_run, SINGLE_CLICK,     BTN_RUN_SINGLE_Click_Handler);
	button_start(&btn_run);
}


void SoftwareTimer_Config(void)
{    
    timer4SetforInt(1);     //1ms¶¨Ê±Æ÷
    timer_init(&swtimer1, swtimer1_callback, 50, 50); 
    timer_start(&swtimer1);
    timer_init(&swtimer2, swtimer2_callback, 5, 5);
    timer_start(&swtimer2);
}


void WS2812B_Config(void)
{
    g_tWS2812B.targetBrightness = WS2812B_CLOSE_BRIGHTNESS;
    g_tWS2812B.currentBrightness = WS2812B_CLOSE_BRIGHTNESS;
    g_tWS2812B.step = WS2812B_STEP;
    g_tWS2812B.status = WS2812B_STOP;
    // g_tWS2812B.setBrightness = setBrightness;
}

void WS2812B_Command(void)
{
    uint8_t i;

    if (g_ucPetalStatus == OPENING)
    {
        if (g_tWS2812B.currentBrightness >= g_tWS2812B.targetBrightness)
        {
            g_tWS2812B.status = WS2812B_STOP;  
            return;
        }
        g_tWS2812B.currentBrightness += g_tWS2812B.step;
    }
    else
    {
        if (g_tWS2812B.currentBrightness <= g_tWS2812B.targetBrightness)
        {
            g_tWS2812B.status = WS2812B_STOP;  
            return;
        }
        g_tWS2812B.currentBrightness -= g_tWS2812B.step;
    }
    
    g_tWS2812B.status = WS2812B_RUNING;  
    // g_tWS2812B.setBrightness(g_tWS2812B.currentBrightness);
    setBrightness(g_tWS2812B.currentBrightness);
    for(i=0; i < NB_LEDS; i++) 
    {
        rgb_SetColor(i,WHITE);
    }
    rgb_SendArray();
}

void BreathLED_Config(void)
{
    g_tBreathLED.targetBrightness = LED_CLOSE_BRIGHTNESS;
    g_tBreathLED.currentBrightness = LED_CLOSE_BRIGHTNESS;
    g_tBreathLED.step = LED_STEP;
    g_tBreathLED.status = BREATHLED_STOP;
    g_tBreathLED.setBrightness = TIM1_SetCompare2;
}

void BreathLED_Command(void)
{
    if (g_ucPetalStatus == OPENING)
    {
        if (g_tBreathLED.currentBrightness >= g_tBreathLED.targetBrightness)
        {
            g_tBreathLED.status = BREATHLED_STOP;  
            return;
        }
        g_tBreathLED.currentBrightness += g_tBreathLED.step;
    }
    else
    {
        if (g_tBreathLED.currentBrightness <= g_tBreathLED.targetBrightness)
        {
            g_tBreathLED.status = BREATHLED_STOP;  
            return;
        }
        g_tBreathLED.currentBrightness -= g_tBreathLED.step;
    }
    
    g_tBreathLED.status = BREATHLED_RUNING;  
    // g_tBreathLED.setBrightness(g_tBreathLED.currentBrightness);
    TIM1_SetCompare2(g_tBreathLED.currentBrightness);
}


void SeverPetal_Config(void)
{
    timer1SetforPWM(PETAL_CLOSE_ANGLE, LED_CLOSE_BRIGHTNESS);
    g_tSeverPetal.targetAngle = PETAL_CLOSE_ANGLE;
    g_tSeverPetal.currentAngle = PETAL_CLOSE_ANGLE;
    g_tSeverPetal.step = PETAL_STEP;
    g_tSeverPetal.status = SEVER_STOP;
    g_tSeverPetal.setAngle = TIM1_SetCompare1;
}

void SeverPetal_Command(void)
{
    if (g_ucPetalStatus == OPENING)
    {
        if (g_tSeverPetal.currentAngle <= g_tSeverPetal.targetAngle)
        {
            g_tSeverPetal.status = SEVER_STOP;  
            return;
        }
        g_tSeverPetal.currentAngle -= g_tSeverPetal.step;

    }
    else
    {
        if (g_tSeverPetal.currentAngle >= g_tSeverPetal.targetAngle)
        {
            g_tSeverPetal.status = SEVER_STOP;  
            return;
        }
        g_tSeverPetal.currentAngle += g_tSeverPetal.step;
    }
    
    g_tSeverPetal.status = SEVER_RUNING;  
    // g_tSeverPetal.setAngle(g_tSeverPetal.currentAngle);
    TIM1_SetCompare1(g_tSeverPetal.currentAngle);
}


void GPIO_Config(void)
{
    GPIO_Init(GPIOF, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);      //LED
    GPIO_Init(GPIOC, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);      //LED1
    GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);      //WS2812b
    GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);      //LED
}


void InitApp(void)
{
    // uint8_t i;

    InitSysclk();
    GPIO_Config();
    Button_Config();
    SoftwareTimer_Config();
    SeverPetal_Config();
    BreathLED_Config();
    WS2812B_Config();


    // setBrightness(10);
    // for(i=0; i < NB_LEDS; i++) 
    // {
    //     rgb_SetColor(i,RED);
    // }
    // rgb_SendArray();

    ENABLE_INT();
}


