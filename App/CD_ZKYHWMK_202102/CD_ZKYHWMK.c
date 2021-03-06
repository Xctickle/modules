

#include "CD_ZKYHWMK.h"


extern void delay_init(void);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);

FONT_T tFont;			/* 定义一个字体结构体变量，用于设置字体参数 */
	char buf[128];


uint16_t Temp;
uint16_t Tobj;

float Temp_f,Tobj_f;
float humi_2,temp_2;

void Delay(uint32_t cnt)
{
	uint32_t i;

	for (i = 0; i < cnt; i++);
}

void read(void)
{

    unsigned char data_temp[2];
    unsigned char p_checksum_temp;




    i2c_Start();
    i2c_SendByte(0x21);
	i2c_WaitAck(); 
    data_temp[0] =  i2c_ReadByte();    //read the first byte (MSB)data_temp[0] =
	i2c_Ack();
    data_temp[1] =  i2c_ReadByte();    //read the second byte (LSB)data_temp[1] =
	i2c_NAck();
    i2c_Stop();

    delay_us(20);


    i2c_Start();
    i2c_SendByte(0x20);
    i2c_WaitAck(); 
    i2c_SendByte(0x84); 
    i2c_WaitAck(); 
    i2c_SendByte(0x00); 
    i2c_Stop();

    delay_us(20);

    do
    {
        i2c_Start();
        i2c_SendByte(0x20);
        i2c_WaitAck(); 
        i2c_SendByte(0x03); 
        i2c_WaitAck(); 
        i2c_SendByte(0x00); 
        i2c_WaitAck(); 
        i2c_Stop();

        delay_ms(100);

        i2c_Start();
        i2c_SendByte(0x21);
        i2c_WaitAck(); 
        data_temp[0] =  i2c_ReadByte();    //read the first byte (MSB)data_temp[0] =
        i2c_Ack();
        data_temp[1] =  i2c_ReadByte();    //read the second byte (LSB)data_temp[1] =
        i2c_NAck();
        i2c_Stop();

        delay_us(20);

    } while (data_temp[0] != 0xa4);
    

    i2c_Start();
    i2c_SendByte(0x20);
	i2c_WaitAck(); 
    i2c_SendByte(0x02); 
	i2c_WaitAck(); 
    i2c_SendByte(0x00); 
	i2c_WaitAck(); 
    i2c_Stop();

    delay_us(20);


    i2c_Start();
    i2c_SendByte(0x21);
	i2c_WaitAck(); 
    data_temp[0] =  i2c_ReadByte();    //read the first byte (MSB)data_temp[0] =
	i2c_Ack();
    data_temp[1] =  i2c_ReadByte();    //read the second byte (LSB)data_temp[1] =
	i2c_NAck();
    i2c_Stop();

    Temp = data_temp[0] << 8;
    Temp += data_temp[1];

    delay_us(20);

    i2c_Start();
    i2c_SendByte(0x20);
	i2c_WaitAck(); 
    i2c_SendByte(0x01); 
	i2c_WaitAck(); 
    i2c_SendByte(0x00); 
	i2c_WaitAck(); 
    i2c_Stop();

    delay_us(20);

    i2c_Start();
    i2c_SendByte(0x21);
	i2c_WaitAck(); 
    data_temp[0] =  i2c_ReadByte();    //read the first byte (MSB)data_temp[0] =
	i2c_Ack();
    data_temp[1] =  i2c_ReadByte();    //read the second byte (LSB)data_temp[1] =
	i2c_NAck();
    i2c_Stop();

    Tobj = data_temp[0] << 8;
    Tobj += data_temp[1];

    Temp_f = Temp / 100.0;
    Tobj_f = Tobj / 100.0;

    // Delay(0x1fffff);

    sprintf(buf, "obj:%d.,ntc:%d ", Tobj, Temp);
    LCD_DispStr(10, 10, buf, &tFont);	

    delay_ms(200);

}



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

    LCD_InitHard();	/* 初始化显示器硬件(配置GPIO和FSMC,给LCD发送初始化指令) */

	LCD_ClrScr(CL_BLUE);  		/* 清屏 背景蓝色 */

	// LCD_SetBackLight(g_tParam.ucBackLight);		/* 设置背光亮度。 */

    // LCD_DispStr(50,50,"tsetsts", &tFont);
    // Tobj = 50;
    // Temp = 20;

    // sprintf(buf, "obj:%d ", Tobj);
    // LCD_DispStr(10, 10, buf, &tFont);	
    // sprintf(buf, "ntc:%d ", Temp);
    // LCD_DispStr(10, 50, buf, &tFont);	
}

