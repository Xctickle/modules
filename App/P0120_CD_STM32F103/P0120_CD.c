

#include "P0120_CD.h"


extern void delay_init(void);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);


#define TOBJ_SAMPLE_NUM 	100

FONT_T tFont;			/* ����һ������ṹ���������������������� */
char buf[64];

uint8_t Draw_Mode = 0;
uint16_t Tobj_Buf[TOBJ_SAMPLE_NUM];

uint16_t Temp;
uint16_t Tobj;

float Temp_f,Tobj_f;

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
		

    	LCD_DispStr(10, 10, buf, &tFont);	
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


void InitApp(void)
{

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_WHITE;	/* ������ɫ */
		tFont.BackColor = RGB(0, 73, 108);		/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}
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

	LCD_ClrScr(CL_BLUE);  		/* ���� ������ɫ */


	LCD_DrawLine(20,20,200,200,CL_GREY2);


	// LCD_SetBackLight(g_tParam.ucBackLight);		/* ���ñ������ȡ� */

    // LCD_DispStr(50,50,"tsetsts", &tFont);
    // Tobj = 50;
    // Temp = 20;

    // sprintf(buf, "obj:%d ", Tobj);
    // LCD_DispStr(10, 10, buf, &tFont);	
    // sprintf(buf, "ntc:%d ", Temp);
    // LCD_DispStr(10, 50, buf, &tFont);	
}

