/******************************************************************************
* @file    uatr.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   STC���߼��㣬ѡ�����ʱ��٣�1Tʱ��,8λ�Զ����ض�ʱ��
*
******************************************************************************
* @attention
*
*******************************************************************************/ 


#include "uart.h"   


#define NONE_PARITY     0       //��У��
#define ODD_PARITY      1       //��У��
#define EVEN_PARITY     2       //żУ��
#define MARK_PARITY     3       //���У��
#define SPACE_PARITY    4       //�հ�У��

#define PARITYBIT ODD_PARITY   //����У��λ

static uint8_t UartSendFlag = 0; //�����жϱ�־λ
static uint8_t UartReceiveFlag = 0; //�����жϱ�־λ


void uartSendByte(uint8_t byte)
{
    ACC = byte;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //����У��λΪ1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //����У��λΪ0
#endif
    }
    SBUF = ACC;                 //д���ݵ�UART���ݼĴ���
    while (!UartSendFlag);               //�ȴ�ǰ������ݷ������
    UartSendFlag = 0;

}

void uartSendString(uint8_t *s)
{
    while ((*s)!='\0')                //����ַ���������־
    {
        uartSendByte(*s);         //���͵�ǰ�ַ�
        s++;
    }
}

/**
  * @brief  19200������@24.000MHz��ѡ��Timer1��Ϊ�������źŷ�����
  * @param  None
  * @retval None
  */
void uart0Init(void) 
{
	P1CON &= 0XF3;
	P1PH |= 0X0C;	 //TX/RXΪ���������룻
	P13 = 1;		 //TX��ʼ�ߵ�ƽ��
	// SCON |= 0X50;     //��ʽ1�������������
    // SCON = 0xd2;                //9λ�ɱ䲨����,У��λ��ʼΪ0

#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8λ�ɱ䲨����
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9λ�ɱ䲨����,У��λ��ʼΪ1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9λ�ɱ䲨����,У��λ��ʼΪ0
#endif

	PCON |= 0X80; 
	T2CON |= 0x00;    //ʹ�ö�ʱ��1��UARTʱ��
	TMOD |= 0X20;     //��ʱ��1  8λ�Զ�����
	TMCON |= 0X02;    //��ʱ��1   Fsys��
	TL1 = 0xB2;		//�趨��ʱ��ֵ
	TH1 = 0xB2;		//�趨��ʱ����װֵ
	TR1 = 1;		 //����Timer0
	// EUART = 1;	     //����UART�ж�
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
