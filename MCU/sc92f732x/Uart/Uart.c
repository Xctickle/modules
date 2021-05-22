/******************************************************************************
* @file    uatr.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   STC工具计算，选波特率倍速，1T时钟,8位自动重载定时器
*
******************************************************************************
* @attention
*
*******************************************************************************/ 


#include "uart.h"   


#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验

#define PARITYBIT ODD_PARITY   //定义校验位

static uint8_t UartSendFlag = 0; //发送中断标志位
static uint8_t UartReceiveFlag = 0; //接收中断标志位


void uartSendByte(uint8_t byte)
{
    ACC = byte;                  //获取校验位P (PSW.0)
    if (P)                      //根据P来设置校验位
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //设置校验位为0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //设置校验位为1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //设置校验位为1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //设置校验位为0
#endif
    }
    SBUF = ACC;                 //写数据到UART数据寄存器
    while (!UartSendFlag);               //等待前面的数据发送完成
    UartSendFlag = 0;

}

void uartSendString(uint8_t *s)
{
    while ((*s)!='\0')                //检测字符串结束标志
    {
        uartSendByte(*s);         //发送当前字符
        s++;
    }
}

/**
  * @brief  19200波特率@24.000MHz，选择Timer1作为波特率信号发生器
  * @param  None
  * @retval None
  */
void uart0Init(void) 
{
	P1CON &= 0XF3;
	P1PH |= 0X0C;	 //TX/RX为带上拉输入；
	P13 = 1;		 //TX初始高电平；
	// SCON |= 0X50;     //方式1，允许接收数据
    // SCON = 0xd2;                //9位可变波特率,校验位初始为0

#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8位可变波特率
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9位可变波特率,校验位初始为1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9位可变波特率,校验位初始为0
#endif

	PCON |= 0X80; 
	T2CON |= 0x00;    //使用定时器1作UART时钟
	TMOD |= 0X20;     //定时器1  8位自动重载
	TMCON |= 0X02;    //定时器1   Fsys；
	TL1 = 0xB2;		//设定定时初值
	TH1 = 0xB2;		//设定定时器重装值
	TR1 = 1;		 //启动Timer0
	// EUART = 1;	     //允许UART中断
}


void uart0(void) interrupt 4
{
	if(TI)
	{
		TI = 0;	
		UartSendFlag = 1;		
	}
	if(RI)
	{
		RI = 0;	
		UartReceiveFlag = 1;
	}	
}

char putchar(char ch)
{
    SBUF = ch;
    while(TI == 0);
    TI = 0;
    return ch;
}

// char putchar (char c)  {
//   while (!TI);
//   TI = 0;
//   return (SBUF = c);
// }
