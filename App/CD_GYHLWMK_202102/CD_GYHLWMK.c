/*
*********************************************************************************************************
*
*	模块名称 : 主程序
*	文件名称 : main.h
*	版    本 : V1.0
*	说    明 : 工业互联网模块
*			1：红外测温功能
*			2：温湿度检测功能
*			3：串口通信
*			
*	修改记录 :
*		版本号   日期       作者    说明
*		v1.0   2021-02-23  xxc  
*
*
*********************************************************************************************************
*/


#include "CD_GYHLWMK.h"
#include "ADC_Command.h"
#include "multi_timer.h"
#include "Timer.h"


#define S_UNKNOW     	0x00
#define S_THERMOPILE 	0x01
#define S_ACCELERATION  0x02
#define S_PRESSURE	 	0x03
#define S_HUMITEMP   	0x04


#define SENSOR_TYPE  S_THERMOPILE
#define SENSOR_ID    4

#define VCC 3.3

//  横坐标环境-20~40，10°C细分
//  纵坐标目标-20~40，10°C细分
const float tp_vol[7][7] =
{
    {1.250, 1.142, 1.015, 0.870, 0.707, 0.526, 0.326}, 
    {1.358, 1.250, 1.123, 0.979, 0.816, 0.634, 0.435}, 
    {1.485, 1.377, 1.250, 1.105, 0.942, 0.761, 0.561}, 
    {1.630, 1.521, 1.395, 1.250, 1.087, 0.906, 0.706}, 
    {1.793, 1.684, 1.558, 1.413, 1.250, 1.069, 0.869}, 
    {1.974, 1.866, 1.739, 1.594, 1.431, 1.250, 1.050}, 
    {2.174, 2.065, 1.939, 1.794, 1.631, 1.450, 1.250}, 
};

const float tobj_idnex[7] = 
{
    -20,-10,0,10,20,30,40
};

// NTC-30~45°C，1°细分 ,3.3v供电
const float ntc_vol[76] =
{
    3.034, 3.019, 3.003, 2.986, 2.969, 2.951, 2.933, 2.913, 2.893, 2.872, 
    2.851, 2.828, 2.805, 2.781, 2.756, 2.731, 2.704, 2.677, 2.649, 2.620, 
    2.591, 2.561, 2.531, 2.499, 2.467, 2.434, 2.401, 2.367, 2.332, 2.296, 
    2.260, 2.222, 2.183, 2.143, 2.104, 2.064, 2.024, 1.983, 1.943, 1.902, 
    1.862, 1.821, 1.781, 1.741, 1.701, 1.661, 1.621, 1.582, 1.543, 1.505, 
    1.467, 1.429, 1.392, 1.355, 1.319, 1.284, 1.249, 1.215, 1.182, 1.149, 
    1.117, 1.085, 1.054, 1.024, 0.995, 0.966, 0.938, 0.910, 0.883, 0.857, 
    0.832, 0.807, 0.783, 0.759, 0.737, 0.714
};

// // NTC-30~45°C，1°细分 ,5v供电
// const float ntc_vol[76] =
// {
//     4.596, 4.574, 4.550, 4.525, 4.499, 4.472, 4.443, 4.414, 4.384, 4.352, 
//     4.319, 4.285, 4.250, 4.214, 4.176, 4.138, 4.098, 4.056, 4.014, 3.970, 
//     3.926, 3.880, 3.834, 3.787, 3.738, 3.688, 3.638, 3.586, 3.533, 3.479, 
//     3.425, 3.366, 3.307, 3.248, 3.188, 3.127, 3.066, 3.005, 2.944, 2.882, 
//     2.821, 2.760, 2.698, 2.637, 2.577, 2.516, 2.457, 2.397, 2.338, 2.280, 
//     2.222, 2.165, 2.109, 2.054, 1.999, 1.946, 1.893, 1.841, 1.791, 1.741, 
//     1.692, 1.644, 1.598, 1.552, 1.507, 1.463, 1.421, 1.379, 1.338, 1.299, 
//     1.260, 1.223, 1.186, 1.151, 1.116, 1.082
// };


float g_fTPobj_L;
float g_fTPobj_H;
float g_fTPntc_H;
float g_fTPntc_L;

float g_fTPntc = 0;
float g_fTPobj = 0;

float g_fTemperature = 0;
float g_fHumidity = 0;

float vtemp_1[7];
float vtemp_2[101];

Timer tTimer_ST1;
Timer tTimer_ST2;
Timer tTimer_ST3;


ADC tADC_TPobj_L;
ADC tADC_TPobj_H;
ADC tADC_TPntc_L;
ADC tADC_TPntc_H;

extern void InitSysclk(void);
extern void timer4SetforInt(uint8_t _ucPeriod);
extern void sht3xDIS_Init(void);
extern uint8_t sht3xDIS_RHMeasure(void);
extern float sht3xDIS_getHumidity(void);
extern float sht3xDIS_getTemperature(void);
extern uint16_t adc_getValue(struct ADC* handle);
extern void adc_loop(void);
extern void adc_stop(struct ADC* handle);
extern int adc_start(struct ADC* handle);
extern void adc_init(struct ADC* handle, void(*ADC_Init_handler)());

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

    GPIO_Init(GPIOB, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);            //输入，ADC

    adc_init(&tADC_TPobj_L,adc_channel5_init);
    adc_init(&tADC_TPobj_H,adc_channel4_init);
    adc_init(&tADC_TPntc_H,adc_channel3_init);
    adc_init(&tADC_TPntc_L,adc_channel2_init);

    adc_start(&tADC_TPobj_L);
    adc_start(&tADC_TPobj_H);
    adc_start(&tADC_TPntc_H);
    adc_start(&tADC_TPntc_L);
}

void get_TP_ntc(void)
{
    uint8_t i,j;
    int16_t t = -300;
    float dtemp;
    float vtemp[11];
    

    dtemp = g_fTPntc_H - g_fTPntc_L;

    for (i = 0; i < 76; i++)
    {
        if (dtemp <= ntc_vol[i] && dtemp >= ntc_vol[i+1])
        {
            t = t + (i*10);
            break;
        }
    }

    vtemp[0] = ntc_vol[i];
    vtemp[10] = ntc_vol[i+1];


    for (j = 1; j < 11; j++)
    {
        vtemp[j] = vtemp[j-1] - ((vtemp[0] - vtemp[10]) / 10.0);
    }

    for (i = 0; i < 11; i++)
    {
        if (dtemp <= vtemp[i] && dtemp >= vtemp[i+1])
        {
            t = t + i;
            break;
        }
    }

    g_fTPntc = t / 10.0;

}

void get_TP_obj(void)
{

    uint8_t i,j;
    int16_t t;
    uint8_t tindex;
    int16_t obj;
    float dev;
    
    t = g_fTPntc + 0.5;
    // t = 15;
    // g_fTPobj_H = 1.432;

    for (i = 0; i < 6; i++)
    {
        if (t >= tobj_idnex[i] && t <= tobj_idnex[i+1])
        {
            tindex = i;
            break;
        }
    }
    
    for (i = 0; i < 7; i++)
    {
        vtemp_1[i] = tp_vol[i][tindex] - ((tp_vol[i][tindex] - tp_vol[i][tindex+1])/10 * (t - tobj_idnex[tindex]));
    }
    
    for (i = 0; i < 7; i++)
    {
        if (g_fTPobj_H >= vtemp_1[i] && g_fTPobj_H <= vtemp_1[i+1])
        {
            obj = tobj_idnex[i] * 10;
            break;
        }
    }

    vtemp_2[0] = vtemp_1[i];
    vtemp_2[10] = vtemp_1[i+1];
    dev = (vtemp_2[10] - vtemp_2[0]) / 100.0;

    for (j = 1; j < 101; j++)
    {
        vtemp_2[j] = vtemp_2[j-1] + dev;
    }

    for (i = 0; i < 101; i++)
    {
        if (g_fTPobj_H >= vtemp_2[i] && g_fTPobj_H <= vtemp_2[i+1])
        {
            obj = obj + i;
            break;
        }
    }

    g_fTPobj = obj / 10.0;
}

void sendData(uint8_t _type, float _pargma1, float _pargma2)
{
    uint8_t i;
    uint16_t p1, p2, sum = 0;
    uint8_t send_data[13];

    p1 = _pargma1 * 10;
    p2 = _pargma2 * 10;

    send_data[0] = 0xAA;
    send_data[1] = _type;
    send_data[2] = SENSOR_ID;
    send_data[3] = p1>>8;
    send_data[4] = p1;
    send_data[5] = p2>>8;
    send_data[6] = p2;
    send_data[7] = 0x00;
    send_data[8] = 0x00;
    send_data[9] = 0x00;
    send_data[10] = 0x00;
    send_data[11] = 0x00;
    send_data[12] = 0x55;

    for (i = 0; i < 10; i++)
    {
        sum  = sum + send_data[i+1];
    }
    send_data[11] = sum;
    for (i = 0; i < 13; i++)
    {
        UART2_SendData8(send_data[i]);
        /* Loop until the end of transmission */
        while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
    }
}

void tTimer_ST1_callback()
{
    uint16_t value;
    value = adc_getValue(&tADC_TPobj_L);
	g_fTPobj_L = (float)value * (VCC / 1024.0); 

    value = adc_getValue(&tADC_TPobj_H);
	g_fTPobj_H = (float)value * (VCC / 1024.0); 

    value = adc_getValue(&tADC_TPntc_H);
	g_fTPntc_H = (float)value * (VCC / 1024.0); 

    value = adc_getValue(&tADC_TPntc_L);
	g_fTPntc_L = (float)value * (VCC / 1024.0); 

	get_TP_ntc();
	get_TP_obj();

    sendData(S_THERMOPILE, g_fTPntc, g_fTPobj);
}

void tTimer_ST2_callback()
{
	adc_loop();
}

void tTimer_ST3_callback()
{
    sht3xDIS_RHMeasure();
    g_fTemperature = sht3xDIS_getTemperature();
    g_fHumidity = sht3xDIS_getHumidity();

    sendData(S_HUMITEMP, g_fTemperature, g_fHumidity);
}


void SoftwareTimer_Config(void)
{    
    timer_init(&tTimer_ST1, tTimer_ST1_callback, 1000, 1000); //1000ms loop
    timer_start(&tTimer_ST1);

    timer_init(&tTimer_ST2, tTimer_ST2_callback, 5, 5); //5ms loop
    timer_start(&tTimer_ST2);

    timer_init(&tTimer_ST3, tTimer_ST3_callback, 1000, 1000); //1000ms loop
    timer_start(&tTimer_ST3);
}

void GPIO_Config(void)
{    

}

/**
  * @brief  Configure UART2 
  * @param  None
  * @retval None
  */
void UART_Config(void)
{
    /* Deinitializes the UART2 peripheral */
    UART2_DeInit();

    /* UART2 configuration -------------------------------------------------*/
    /* UART2 configured as follow:
          - BaudRate = 9600 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Receive and transmit enabled
          - UART2 Clock disabled
     */
    /* Configure the UART2 */
    UART2_Init((uint32_t)9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO,
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    

    /* Enable UART2 Receive Transmit interrupt */
    // UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);

}

void InitApp(void)
{
    InitSysclk();
    GPIO_Config();
	UART_Config();
	timer4SetforInt(1);
    SoftwareTimer_Config();
    adc_config();
    sht3xDIS_Init();

    ENABLE_INT();

}

