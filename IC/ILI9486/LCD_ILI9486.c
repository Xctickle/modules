/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : LCD_ILI9486.c
*	版    本 : V1.0
*	说    明 : ILI9486 显示器分辨率为 480 * 320,  3.5寸普通比例4：3
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2014-07-26 armfly  首版
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "LCD_ILI9486.h"

#define	LCD_LED PBout(0) //LCD背光    		 PB0 

#define	LCD_RST_SET	GPIOC->BSRR=1<<5    //复位			PC5
								    
#define	LCD_RST_CLR	GPIOC->BRR=1<<5    //复位			PC5	

#define ILI9486_BASE       ((uint32_t)(0x6C000000 | 0x00000000))

#define ILI9486_REG		*(__IO uint16_t *)(ILI9486_BASE)
#define ILI9486_RAM		*(__IO uint16_t *)(ILI9486_BASE + (1 << (10 + 1)))	/* FSMC 16位总线模式下，FSMC_A10口线对应物理地址A11 */

static __IO uint8_t s_RGBChgEn = 0;		/* RGB转换使能, 4001屏写显存后读会的RGB格式和写入的不同 */

static void Init_9486(void);


/*
*********************************************************************************************************
*	函 数 名: ILI9486_InitHard
*	功能说明: 初始化LCD
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_InitHard(void)
{
	uint32_t id;

	id = ILI9486_ReadID();

	// if (id == 0x548066)
	{
		Init_9486();	/* 初始化5420和4001屏硬件 */

		//ILI9486_WriteCmd(0x23);
		//ILI9486_WriteCmd(0x22);

		s_RGBChgEn = 0;

		ILI9486_PutPixel(1,1, 0x12);
		g_ChipID = ILI9486_GetPixel(1,1);

		ILI9486_PutPixel(1,1, 0x34);
		g_ChipID = ILI9486_GetPixel(1,1);

		ILI9486_PutPixel(1,1, 0x56);
		g_ChipID = ILI9486_GetPixel(1,1);

		g_ChipID = IC_9486;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_SetDirection
*	功能说明: 设置显示方向。
*	形    参:  _ucDir : 显示方向代码 0 横屏正常, 1=横屏180度翻转, 2=竖屏, 3=竖屏180度翻转
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_SetDirection(uint8_t _ucDir)
{
	
	/*
		Memory Access Control (36h)
		This command defines read/write scanning direction of the frame memory.

		These 3 bits control the direction from the MPU to memory write/read.

		Bit  Symbol  Name  Description
		D7   MY  Row Address Order
		D6   MX  Column Address Order
		D5   MV  Row/Column Exchange
		D4   ML  Vertical Refresh Order  LCD vertical refresh direction control. 、

		D3   BGR RGB-BGR Order   Color selector switch control
		     (0 = RGB color filter panel, 1 = BGR color filter panel )
		D2   MH  Horizontal Refresh ORDER  LCD horizontal refreshing direction control.
		D1   X   Reserved  Reserved
		D0   X   Reserved  Reserved
	*/
	ILI9486_WriteCmd(0x36);
	/* 0 表示竖屏(排线在下)，1表示竖屏(排线在上), 2表示横屏(排线在左边)  3表示横屏 (排线在右边) */
	if (_ucDir == 0)
	{
		ILI9486_WriteParam(0xA8);	/* 横屏(排线在左边) */
		g_LcdHeight = 320;
		g_LcdWidth = 480;
	}
	else if (_ucDir == 1)
	{
		ILI9486_WriteParam(0x68);	/* 横屏 (排线在右边) */
		g_LcdHeight = 320;
		g_LcdWidth = 480;
	}
	else if (_ucDir == 2)
	{
		ILI9486_WriteParam(0xC8);	/* 竖屏(排线在上) */
		g_LcdHeight = 480;
		g_LcdWidth = 320;
	}
	else if (_ucDir == 3)
	{
		ILI9486_WriteParam(0x08);	/* 竖屏(排线在下) */
		g_LcdHeight = 480;
		g_LcdWidth = 320;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Init_9486
*	功能说明: 初始化ILI9486驱动器。扫描方式：从左向右，从上到下
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Init_9486(void)
{
	/* 初始化LCD，写LCD寄存器进行配置 */

	//************* Start Initial Sequence **********//

	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);

	//************* Start Initial Sequence **********//		
	ILI9486_WriteCmd(0XF9);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x08);
	
	ILI9486_WriteCmd(0xC0);
	ILI9486_WriteParam(0x19);//VREG1OUT POSITIVE
	ILI9486_WriteParam(0x1a);//VREG2OUT NEGATIVE
	
	ILI9486_WriteCmd(0xC1);
	ILI9486_WriteParam(0x45);//VGH,VGL    VGH>=14V.
	ILI9486_WriteParam(0x00);
	
	ILI9486_WriteCmd(0xC2);
	ILI9486_WriteParam(0x33);
	
	ILI9486_WriteCmd(0XC5);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x28);//VCM_REG[7:0]. <=0X80.
	
	ILI9486_WriteCmd(0xB1);
	ILI9486_WriteParam(0xA0);//0XB0 =70HZ, <=0XB0.0xA0=62HZ
	ILI9486_WriteParam(0x11);
	
	ILI9486_WriteCmd(0xB4);
	ILI9486_WriteParam(0x02); //2 DOT FRAME MODE,F<=70HZ.
	
	ILI9486_WriteCmd(0xB6);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x42);//0 GS SS SM ISC[3:0];
	ILI9486_WriteParam(0x3B);
	
	
	ILI9486_WriteCmd(0xB7);
	ILI9486_WriteParam(0x07);
	
	ILI9486_WriteCmd(0xE0);
	ILI9486_WriteParam(0x1F);
	ILI9486_WriteParam(0x25);
	ILI9486_WriteParam(0x22);
	ILI9486_WriteParam(0x0B);
	ILI9486_WriteParam(0x06);
	ILI9486_WriteParam(0x0A);
	ILI9486_WriteParam(0x4E);
	ILI9486_WriteParam(0xC6);
	ILI9486_WriteParam(0x39);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x00);
	
	ILI9486_WriteCmd(0XE1);
	ILI9486_WriteParam(0x1F);
	ILI9486_WriteParam(0x3F);
	ILI9486_WriteParam(0x3F);
	ILI9486_WriteParam(0x0F);
	ILI9486_WriteParam(0x1F);
	ILI9486_WriteParam(0x0F);
	ILI9486_WriteParam(0x46);
	ILI9486_WriteParam(0x49);
	ILI9486_WriteParam(0x31);
	ILI9486_WriteParam(0x05);
	ILI9486_WriteParam(0x09);
	ILI9486_WriteParam(0x03);
	ILI9486_WriteParam(0x1C);
	ILI9486_WriteParam(0x1A);
	ILI9486_WriteParam(0x00);
	
	ILI9486_WriteCmd(0XF1);
	ILI9486_WriteParam(0x36);
	ILI9486_WriteParam(0x04);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x3C);
	ILI9486_WriteParam(0x0F);
	ILI9486_WriteParam(0x0F);
	ILI9486_WriteParam(0xA4);
	ILI9486_WriteParam(0x02);
	
	ILI9486_WriteCmd(0XF2);
	ILI9486_WriteParam(0x18);
	ILI9486_WriteParam(0xA3);
	ILI9486_WriteParam(0x12);
	ILI9486_WriteParam(0x02);
	ILI9486_WriteParam(0x32);
	ILI9486_WriteParam(0x12);
	ILI9486_WriteParam(0xFF);
	ILI9486_WriteParam(0x32);
	ILI9486_WriteParam(0x00);
	
	ILI9486_WriteCmd(0XF4);
	ILI9486_WriteParam(0x40);
	ILI9486_WriteParam(0x00);
	ILI9486_WriteParam(0x08);
	ILI9486_WriteParam(0x91);
	ILI9486_WriteParam(0x04);
	
	ILI9486_WriteCmd(0XF8);
	ILI9486_WriteParam(0x21);
	ILI9486_WriteParam(0x04);
	
	ILI9486_WriteCmd(0x36);
	ILI9486_WriteParam(0x48);
	// ILI9486_WriteParam(0x08 |(6<<5));//根据ucOption的值设置LCD参数，共0-7种模式
	
	ILI9486_WriteCmd(0x3A);
	ILI9486_WriteParam(0x55);
	
	ILI9486_WriteCmd(0x11);
	delay_ms(120);
	ILI9486_WriteCmd(0x29);

	LCD_LED=1;//点亮背光	 

#if 1
	/* 设置显示窗口 */
	ILI9486_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_WriteCmd
*	功能说明: 向LCD控制器芯片发送命令
*	形    参: _ucCmd : 命令代码
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_WriteCmd(uint8_t _ucCmd)
{
	ILI9486_REG = _ucCmd;	/* 发送CMD */
}


/*
*********************************************************************************************************
*	函 数 名: ILI9486_WriteParam
*	功能说明: 向LCD控制器芯片发送参数(data)
*	形    参: _ucParam : 参数数据
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_WriteParam(uint8_t _ucParam)
{
	ILI9486_RAM = _ucParam;
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_SetDispWin
*	功能说明: 设置显示窗口，进入窗口显示模式。TFT驱动芯片支持窗口显示模式，这是和一般的12864点阵LCD的最大区别。
*	形    参:
*		_usX : 水平坐标
*		_usY : 垂直坐标
*		_usHeight: 窗口高度
*		_usWidth : 窗口宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth)
{
	ILI9486_WriteCmd(0X2A); 		/* 设置X坐标 */
	ILI9486_WriteParam(_usX >> 8);	/* 起始点 */
	ILI9486_WriteParam(_usX);
	ILI9486_WriteParam((_usX + _usWidth - 1) >> 8);	/* 结束点 */
	ILI9486_WriteParam(_usX + _usWidth - 1);

	ILI9486_WriteCmd(0X2B); 				  /* 设置Y坐标*/
	ILI9486_WriteParam(_usY >> 8);   /* 起始点 */
	ILI9486_WriteParam(_usY);
	ILI9486_WriteParam((_usY + _usHeight - 1) >>8);		/* 结束点 */
	ILI9486_WriteParam((_usY + _usHeight - 1));
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_SetCursor
*	功能说明: 设置光标位置
*	形    参:  _usX : X坐标; _usY: Y坐标
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_SetCursor(uint16_t _usX, uint16_t _usY)
{
	ILI9486_WriteCmd(0X2A); 		/* 设置X坐标 */
	ILI9486_WriteParam(_usX >> 8);	/* 先高8位，然后低8位 */
	ILI9486_WriteParam(_usX);		/* 设置起始点和结束点*/
	ILI9486_WriteParam(_usX >> 8);	/* 先高8位，然后低8位 */
	ILI9486_WriteParam(_usX);		/* 设置起始点和结束点*/

    ILI9486_WriteCmd(0X2B); 		/* 设置Y坐标*/
	ILI9486_WriteParam(_usY >> 8);
	ILI9486_WriteParam(_usY);
	ILI9486_WriteParam(_usY >> 8);
	ILI9486_WriteParam(_usY);
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_QuitWinMode
*	功能说明: 退出窗口显示模式，变为全屏显示模式
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_QuitWinMode(void)
{
	ILI9486_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_ReadID
*	功能说明: 读取LCD驱动芯片ID， 4个bit
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
uint32_t ILI9486_ReadID(void)
{
	uint8_t buf[4];

	ILI9486_REG = 0x04;
	buf[0] = ILI9486_RAM;
	buf[1] = ILI9486_RAM;
	buf[2] = ILI9486_RAM;
	buf[3] = ILI9486_RAM;

	return (buf[1] << 16) + (buf[2] << 8) + buf[3];
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_DispOn
*	功能说明: 打开显示
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DispOn(void)
{
	;
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_DispOff
*	功能说明: 关闭显示
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DispOff(void)
{
	;
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_ClrScr
*	功能说明: 根据输入的颜色值清屏
*	形    参:  _usColor : 背景色
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_ClrScr(uint16_t _usColor)
{
	uint32_t i;
	uint32_t n;

	ILI9486_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);

	ILI9486_REG = 0x2C; 			/* 准备读写显存 */

#if 1		/* 优化代码执行速度 */
	n = (g_LcdHeight * g_LcdWidth) / 8;
	for (i = 0; i < n; i++)
	{
		ILI9486_RAM = _usColor;
		ILI9486_RAM = _usColor;
		ILI9486_RAM = _usColor;
		ILI9486_RAM = _usColor;

		ILI9486_RAM = _usColor;
		ILI9486_RAM = _usColor;
		ILI9486_RAM = _usColor;
		ILI9486_RAM = _usColor;
	}
#else
	n = g_LcdHeight * g_LcdWidth;
	for (i = 0; i < n; i++)
	{
		ILI9486_RAM = _usColor;
	}
#endif

}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_PutPixel
*	功能说明: 画1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	ILI9486_SetCursor(_usX, _usY);	/* 设置光标位置 */

	/* 写显存 */
	ILI9486_REG = 0x2C;
	ILI9486_RAM = _usColor;
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_GetPixel
*	功能说明: 读取1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: RGB颜色值 （565）
*********************************************************************************************************
*/
uint16_t ILI9486_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t R = 0, G = 0, B = 0 ;

	ILI9486_SetCursor(_usX, _usY);	/* 设置光标位置 */

	ILI9486_REG = 0x2E;
	R = ILI9486_RAM; 	/* 第1个哑读，丢弃 */
	R = ILI9486_RAM;
	B = ILI9486_RAM;
	G = ILI9486_RAM;

    return (((R >> 11) << 11) | ((G >> 10 ) << 5) | (B >> 11));
}


/**
 * @brief  在 ILI9341 显示器上显示一个英文字符
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下该点的起始Y坐标
 * @param  usY ：在特定扫描方向下该点的起始Y坐标
 * @param  cChar ：要显示的英文字符
 * @note 
 * @retval 无
 */
void ILI9486_DispChar_EN ( uint16_t usX, uint16_t usY, sFONT _sfont, const char cChar, uint16_t _textColor, uint16_t _backColor)
{
	uint8_t  byteCount, bitCount,fontLength;	
	uint16_t ucRelativePositon;
	uint8_t *Pfont;
	
	//对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
	ucRelativePositon = cChar - ' ';
	
	//每个字模的字节数
	fontLength = (_sfont.Width*_sfont.Height)/8;
		
	//字模首地址
	/*ascii码表偏移值乘以每个字模的字节数，求出字模的偏移位置*/
	Pfont = (uint8_t *)&_sfont.table[ucRelativePositon * fontLength];
	
	//设置显示窗口
	ILI9486_SetDispWin ( usX, usY, _sfont.Height, _sfont.Width);
	
	ILI9486_WriteCmd (0x2C);			

	//按字节读取字模数据
	//由于前面直接设置了显示窗口，显示数据会自动换行
	for (byteCount = 0; byteCount < fontLength; byteCount++)
	{
		//一位一位处理要显示的颜色
		for (bitCount = 0; bitCount < 8; bitCount++)
		{
			if (Pfont[byteCount] & (0x80>>bitCount))
				ILI9486_RAM = _textColor;	
			else
				ILI9486_RAM = _backColor;	
		}	
	}	
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_DrawLine
*	功能说明: 采用 Bresenham 算法，在2点间画一条直线。
*	形    参:
*			_usX1, _usY1 ：起始点坐标
*			_usX2, _usY2 ：终止点Y坐标
*			_usColor     ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* 采用 Bresenham 算法，在2点间画一条直线 */

	ILI9486_PutPixel(_usX1 , _usY1 , _usColor);

	/* 如果两点重合，结束后面的动作。*/
	if ( _usX1 == _usX2 && _usY1 == _usY2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (_usX2 >= _usX1)
	{
		dx = _usX2 - _usX1;
	}
	else
	{
		dx = _usX1 - _usX2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (_usY2 >= _usY1)
	{
		dy = _usY2 - _usY1;
	}
	else
	{
		dy = _usY1 - _usY2;
	}

	if ( dx < dy )   /*如果dy为计长方向，则交换纵横坐标。*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = _usX1; _usX1 = _usY1; _usY1 = temp;
		temp = _usX2; _usX2 = _usY2; _usY2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1 ;    /* 确定是增1还是减1 */
	ty = _usY2 > _usY1 ? 1 : -1 ;
	x = _usX1 ;
	y = _usY1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != _usX2 )     /* 循环画点 */
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if ( iTag )
		{
			ILI9486_PutPixel ( y , x , _usColor) ;
		}
		else
		{
			ILI9486_PutPixel ( x , y , _usColor) ;
		}
		x += tx ;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_DrawHLine
*	功能说明: 绘制一条水平线 （主要用于UCGUI的接口函数）
*	形    参:  _usX1    ：起始点X坐标
*			  _usY1    ：水平线的Y坐标
*			  _usX2    ：结束点X坐标
*			  _usColor : 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor)
{
	uint16_t i;

	ILI9486_SetDispWin(_usX1, _usY1, 1, _usX2 - _usX1 + 1);

	ILI9486_REG = 0x2C;

	/* 写显存 */
	for (i = 0; i <_usX2-_usX1 + 1; i++)
	{
		ILI9486_RAM = _usColor;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_DrawHColorLine
*	功能说明: 绘制一条彩色水平线 （主要用于UCGUI的接口函数）
*	形    参:  _usX1    ：起始点X坐标
*			  _usY1    ：水平线的Y坐标
*			  _usWidth ：直线的宽度
*			  _pColor : 颜色缓冲区
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor)
{
	uint16_t i;

	ILI9486_SetCursor(_usX1, _usY1);

	/* 写显存 */
	ILI9486_REG = 0x2C;
	for (i = 0; i < _usWidth; i++)
	{
		ILI9486_RAM = *_pColor++;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_DrawHTransLine
*	功能说明: 绘制一条彩色透明的水平线 （主要用于UCGUI的接口函数）， 颜色值为0表示透明色
*	形    参:  _usX1    ：起始点X坐标
*			  _usY1    ：水平线的Y坐标
*			  _usWidth ：直线的宽度
*			  _pColor : 颜色缓冲区
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DrawHTransLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor)
{
	uint16_t i, j;
	uint16_t Index;

	ILI9486_SetCursor(_usX1, _usY1);

	/* 写显存 */
	ILI9486_REG = 0x2C;
	for (i = 0,j = 0; i < _usWidth; i++, j++)
	{
		Index = *_pColor++;
	    if (Index)
        {
			 ILI9486_RAM = Index;
		}
		else
		{
			ILI9486_SetCursor(_usX1 + j, _usY1);
			ILI9486_REG = 0x2C;
			ILI9486_RAM = Index;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_DrawRect
*	功能说明: 绘制水平放置的矩形。
*	形    参:
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	/*
	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	ILI9486_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _usColor);	/* 顶 */
	ILI9486_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _usColor);	/* 底 */

	ILI9486_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _usColor);	/* 左 */
	ILI9486_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _usColor);	/* 右 */
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_FillRect
*	功能说明: 填充矩形。
*	形    参:
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	uint32_t i;

	/*
	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	ILI9486_SetDispWin(_usX, _usY, _usHeight, _usWidth);

	ILI9486_REG = 0x2C;
	for (i = 0; i < _usHeight * _usWidth; i++)
	{
		ILI9486_RAM = _usColor;
	}
}


/*
*********************************************************************************************************
*	函 数 名: ILI9486_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参:
*			_usX,_usY  ：圆心的坐标
*			_usRadius  ：圆的半径
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* 当前 X 值 */
	uint32_t  CurY;		/* 当前 Y 值 */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		ILI9486_PutPixel(_usX + CurX, _usY + CurY, _usColor);
		ILI9486_PutPixel(_usX + CurX, _usY - CurY, _usColor);
		ILI9486_PutPixel(_usX - CurX, _usY + CurY, _usColor);
		ILI9486_PutPixel(_usX - CurX, _usY - CurY, _usColor);
		ILI9486_PutPixel(_usX + CurY, _usY + CurX, _usColor);
		ILI9486_PutPixel(_usX + CurY, _usY - CurX, _usColor);
		ILI9486_PutPixel(_usX - CurY, _usY + CurX, _usColor);
		ILI9486_PutPixel(_usX - CurY, _usY - CurX, _usColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ILI9486_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下
*	形    参:
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void ILI9486_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	uint32_t index = 0;
	const uint16_t *p;

	/* 设置图片的位置和大小， 即设置显示窗口 */
	ILI9486_SetDispWin(_usX, _usY, _usHeight, _usWidth);

	p = _ptr;
	for (index = 0; index < _usHeight * _usWidth; index++)
	{
		ILI9486_PutPixel(_usX, _usY, *p++);
	}

	/* 退出窗口绘图模式 */
	ILI9486_QuitWinMode();
}


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
