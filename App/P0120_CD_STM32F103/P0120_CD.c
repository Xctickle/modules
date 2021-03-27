

#include "P0120_CD.h"


extern void delay_init(void);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);


#define TOBJ_SAMPLE_NUM 	100

FONT_T tFont;			/* 定义一个字体结构体变量，用于设置字体参数 */
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
			_pBuf[pos++] = ucData;		/* 保存接收到的数据 */
			if (pos == 7)
			{
				_pBuf[pos] = 0;	/* 结尾加0， 便于函数调用者识别字符串结束 */
				ret = pos;		/* 成功。 返回数据长度 */
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

	/* 设置字体参数 */
	{
		tFont.FontCode = FC_ST_16;		/* 字体代码 16点阵 */
		tFont.FrontColor = CL_WHITE;	/* 字体颜色 */
		tFont.BackColor = RGB(0, 73, 108);		/* 文字背景颜色 */
		tFont.Space = 0;				/* 文字间距，单位 = 像素 */
	}
    	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

		系统时钟缺省配置为168MHz，如果需要更改，可以修改 system_stm32f4xx.c 文件
	*/

	/* 优先级分组设置为4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    delay_init();

    InitI2C();

	bsp_InitUart();		/* 初始化串口驱动 */

    LCD_InitHard();	/* 初始化显示器硬件(配置GPIO和FSMC,给LCD发送初始化指令) */

	LCD_ClrScr(CL_BLUE);  		/* 清屏 背景蓝色 */


	LCD_DrawLine(20,20,200,200,CL_GREY2);


	// LCD_SetBackLight(g_tParam.ucBackLight);		/* 设置背光亮度。 */

    // LCD_DispStr(50,50,"tsetsts", &tFont);
    // Tobj = 50;
    // Temp = 20;

    // sprintf(buf, "obj:%d ", Tobj);
    // LCD_DispStr(10, 10, buf, &tFont);	
    // sprintf(buf, "ntc:%d ", Temp);
    // LCD_DispStr(10, 50, buf, &tFont);	
}

