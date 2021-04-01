

#include "P0120_CD.h"


extern void delay_init(void);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);


#define CHART_H_MAX 200 
#define CHART_H_MIN 100
#define CHART_V_MAX 300
#define CHART_V_MIN 100

#define TOBJ_SAMPLE_NUM 100


// FONT_T tFont;			/* ����һ������ṹ���������������������� */
char buf[64];

uint8_t Draw_Mode = 0;
uint16_t Tobj_Buf[TOBJ_SAMPLE_NUM] = {250};
uint16_t waveBuf[TOBJ_SAMPLE_NUM] = {0}; //�������������Ĳ�������

uint16_t Temp;
uint16_t Tobj;

uint16_t Tobj_max = 0;
uint16_t Tobj_min = 0;
uint16_t Tobj_agv = 0;



float Temp_f,Tobj_f;

uint16_t err_cnt = 0;


void PlotWave(void);


uint16_t ReadLine(char *_pBuf)
{
	uint8_t ucData;
	uint16_t pos = 0;
	uint8_t ret;
	uint16_t cnt = 0;


	while (1)
	{
		if (comGetChar(COM1, &ucData))
		{
			_pBuf[pos++] = ucData;		/* ������յ������� */
			if (pos == 7)
			{
				_pBuf[pos] = 0;	/* ��β��0�� ���ں���������ʶ���ַ������� */
				ret = pos;		/* �ɹ��� �������ݳ��� */
				break;
			}
		}
		delay_ms(10);
		cnt++;
		if (cnt > 200)
		{
			return 0;
		}
		
	}
	return ret;
}

void read(void)
{
    printf("updataObj");
    delay_ms(300);
	if (ReadLine(buf))
	{
		if (str_len(buf) == 7)
		{
			Tobj = str_to_int(buf+3);
		}
		
    	// LCD_DispStr(10, 10, buf, &tFont);	
	}

	ArrayDataShift(Tobj+250, Tobj_Buf, TOBJ_SAMPLE_NUM);

	PlotWave();

	if (Tobj_max == 0)
	{
		Tobj_max = Tobj;
	}
	if (Tobj_min == 0)
	{
		Tobj_min = Tobj;
	}	

	if(Tobj > Tobj_max)
	{
		Tobj_max = Tobj;
	}
	else if(Tobj < Tobj_min)
	{
		Tobj_min = Tobj;
	}


    sprintf(buf, "obj:%.1f C", (float)Tobj/10.0);
	ILI9341_DispString_EN(20,50,buf);

    sprintf(buf, "max:%.1f C", (float)Tobj_max/10.0);
	ILI9341_DispString_EN(20,80,buf);

    sprintf(buf, "min:%.1f C", (float)Tobj_min/10.0);
	ILI9341_DispString_EN(20,110,buf);

}

void PlotWave(void)
{
	uint16_t i = 0;
	
	for (i = 0; i < (CHART_H_MAX - CHART_H_MIN - 1); i++)
	{
		LCD_DrawLine(i + CHART_H_MIN, waveBuf[i], i + CHART_H_MIN + 1, waveBuf[i + 1],CL_BLUE);
	}

	for (i = 0; i < TOBJ_SAMPLE_NUM; i++)
    {
        waveBuf[i] = DataRemap_reversal(*(Tobj_Buf + i), 1000, 0, CHART_V_MAX, CHART_V_MIN);
    }

	for (i = 0; i < (CHART_H_MAX - CHART_H_MIN - 1); i++)
	{
		LCD_DrawLine(i + CHART_H_MIN, waveBuf[i], i + CHART_H_MIN + 1, waveBuf[i + 1],CL_GREY4);
	}

}

// void PlotWave(void)
// {
//     uint8 i;
//     if (Draw_Mode == 0) //Line Mode
//     {
//         for (i = 0; i < 100; i++)
//         {
//             LCD_DrawLine(i + 26, *(WaveData - 50 + i), i + 27, *(WaveData - 50 + i + 1));
//         }
//     }
//     if (Draw_Mode == 1) //Dot Mode
//     {
//         for (i = 0; i <= 100; i++)
//         {
//             LCD_DrawPoints(i + 26, *(WaveData - 50 + i));
//         }
//     }
// }

/**
  * @brief  Configure PA.00 in interrupt mode
  * @param  None
  * @retval None
  */
void EXTI0_Config(void)
{

	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configure PA.00 pin as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Connect EXTI0 Line to PA.00 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

	/* Configure EXTI0 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void InitApp(void)
{

	// /* ����������� */
	// {
	// 	tFont.FontCode = FC_ST_16;		/* ������� 16���� */
	// 	tFont.FrontColor = CL_WHITE;	/* ������ɫ */
	// 	tFont.BackColor = RGB(0, 73, 108);		/* ���ֱ�����ɫ */
	// 	tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	// }
    	/*
		����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
		�����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����

		ϵͳʱ��ȱʡ����Ϊ168MHz�������Ҫ���ģ������޸� system_stm32f4xx.c �ļ�
	*/

	/* ���ȼ���������Ϊ4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    delay_init();

    InitI2C();

	bsp_InitUart();		/* ��ʼ���������� */

    LCD_InitHard();	/* ��ʼ����ʾ��Ӳ��(����GPIO��FSMC,��LCD���ͳ�ʼ��ָ��) */

	// EXTI0_Config();

	LCD_ClrScr(CL_BLUE);  		/* ���� ������ɫ */

	LCD_SetFont(&Font24x32);
	LCD_SetColors(CL_GREEN,CL_WHITE);

	// ILI9341_DispString_EN(20,50,"0123456");



	// LCD_SetBackLight(g_tParam.ucBackLight);		/* ���ñ������ȡ� */

    // LCD_DispStr(50,50,"tsetsts", &tFont);
    // Tobj = 50;
    // Temp = 20;

    // sprintf(buf, "obj:%d ", Tobj);
    // LCD_DispStr(10, 10, buf, &tFont);	
    // sprintf(buf, "ntc:%d ", Temp);
    // LCD_DispStr(10, 50, buf, &tFont);	
}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
	  err_cnt++;

    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}