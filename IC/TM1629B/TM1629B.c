/******************************************************************************
* @file    TM1629B.c
* @author  xxc
* @version V1.0
* @date    2020-12-15
* @brief   
*
******************************************************************************
* @attention
*
*******************************************************************************/ 

#include "TM1629B.h"   



// #define	TM1629B_STB_1_SET 	 		(GPIO_WriteHigh(GPIOA, GPIO_PIN_5))
// #define	TM1629B_CLK_1_SET 	 		(GPIO_WriteHigh(GPIOA, GPIO_PIN_4))
// #define	TM1629B_DIN_1_SET 	 		(GPIO_WriteHigh(GPIOA, GPIO_PIN_3))

// #define	TM1629B_STB_1_CLR 	 		(GPIO_WriteLow(GPIOA, GPIO_PIN_5))
// #define	TM1629B_CLK_1_CLR 	 		(GPIO_WriteLow(GPIOA, GPIO_PIN_4))
// #define	TM1629B_DIN_1_CLR 	 		(GPIO_WriteLow(GPIOA, GPIO_PIN_3))

#define	TM1629B_STB_1_SET 	 		P21 = 1
#define	TM1629B_CLK_1_SET 	 		P20 = 1
#define	TM1629B_DIN_1_SET 	 		P03 = 1

#define	TM1629B_STB_1_CLR 	 		P21 = 0
#define	TM1629B_CLK_1_CLR 	 		P20 = 0
#define	TM1629B_DIN_1_CLR 	 		P03 = 0

#define	TM1629B_CLK_CLR 	 	TM1629B_CLK_1_CLR;

#define	TM1629B_DIN_CLR 	 	TM1629B_DIN_1_CLR; 

#define	TM1629B_CLK_SET 	 	TM1629B_CLK_1_SET; 

#define	TM1629B_DIN_SET 	 	TM1629B_DIN_1_SET; 




uint8_t TM1629B_buff_1[8][2] = {0x00};
uint8_t TM1629B_buff_2[8][2] = {0x00};

void convert1(unsigned char*pSrcBuf,unsigned char*pDstBuf,unsigned char n);
extern void DelayUs(uint16_t _us);


// static void DelayUs(uint16_t _us)
// {
// 	uint16_t i,a;

// 	for (i = 0; i < _us; i++)
// 	{
// 		a = 3;
// 		while (--a);
// 	}
// }

void TM1629B_Clear_Data(void);

/**
* @brief    ��TM1629B��ʼ��
* @input    ��-none  
* @output   ��-none
* @retval   ��-none
**/
void TM1629B_Init(void)
{

	TM1629B_Clear_Data();
}


/**
* @brief    �����TM1629B�Ĵ���
* @input    ��-none  
* @output   ��-none
* @retval   ��-none
**/
void TM1629B_Clear_Data(void)
{
	uint8_t i = 0; 

	// for(i = 0; i < 16; i++)
	// {
	// 	// TM1629B_buff[i] = 0x00; //��ʾ�������������ʾ����.  
	// 	*(TM1629B_buff_2+i) = 0x00;
	// }

}




/**
* @brief    ��д��һ���ֽ�
* @input    ��-dat д����ֽ�  
* @output   ��-none
* @retval   ��-none
**/
void TM1629B_W_byte(uint8_t dat)   
{ 
	uint8_t i = 0;  


	DelayUs(1); 
	TM1629B_DIN_SET;

	DelayUs(1); 

	for(i = 0;i < 8; i++)
	{   
		if(dat & 0x01)
		{
			TM1629B_DIN_SET;
		}
		else
		{ 
			TM1629B_DIN_CLR; 
		}
										
		TM1629B_CLK_CLR;  
		DelayUs(2);                     
		TM1629B_CLK_SET; 
		DelayUs(2);    
		dat  >>=  1;           
	} 
	DelayUs(1);      	 
}


void TM1629B_1_Display(void)
{
	uint8_t i = 0;

	TM1629B_STB_1_SET;
	DelayUs(1);                     
	TM1629B_STB_1_CLR; 

	TM1629B_W_byte(0x40);                     //������ַ

	TM1629B_STB_1_SET;
	DelayUs(1);                     
	TM1629B_STB_1_CLR; 

	TM1629B_W_byte(0xC0 | 0x00);              //���õ�ַ  
				
	for(i = 0; i < 8; i++)
	{ 
		// TM1629B_W_byte(*(TM1629B_buff_2+i));     
		// TM1629B_W_byte(0xff);     
		TM1629B_W_byte(TM1629B_buff_2[i][0]);     
		TM1629B_W_byte(TM1629B_buff_2[i][1]);     
	} 





	TM1629B_STB_1_SET;
	DelayUs(1);                     
	TM1629B_STB_1_CLR; 

	TM1629B_W_byte(0x8A);          //��ʾ�� ,�������.           
	TM1629B_STB_1_SET;  

} 


// void TM1629B_BitChange(void)
// {  
//     uint8_t i;         

// 	for(i = 0; i < 15; i++)
// 	{ 
//     	TM1629B_DA[i].byte = TM1629B_buff[i].byte;
// 	}          

//     // for(i = 0;i < 4;i++)
//     // {
//     //     TM1629B_buff[i].byte = 0;
//     // }
//     // TM1629B_dB.byte = 0;
// }

// void L_to_H()  //���ڹ�����ת��Ϊ��������ʾ �� SEG��λѡ GRID�Ӷ�ѡ 16λͨ��
// {
// 	uchar i,j;
// 	for(i=0;i<16;i++)
// 	{
// 		for(j=0;j<8;j++)
// 		{
// 			if(i<8)   //����SEG1-8����˳��
// 			{
// 				temp2[j*2]>>=1;                        //�ƿ����λ�ȴ�����
// 				if((temp&(1<<j)))        //ȡ��������ĳһλ�ж��Ƿ�Ϊ��1��
// 				{
// 						temp2[j*2]|=0x80;        //Ϊ��1�������λ����1 ����������
// 				}
// 			}
// 			else         //����SEG9-16����˳��
// 			{
// 				temp2[j*2+1]>>=1;
// 				if((temp&(1<<j)))
// 				{
// 						temp2[j*2+1]|=0x80;
// 				}
// 			}
// 		}
// 	}
// }

/**********************************************************
��������ת���ɹ�������
pSrcBuf����Ź�������
pDstBuf����Ź�������
n��ת������
**********************************************************/
void convert1(unsigned char*pSrcBuf,unsigned char*pDstBuf,unsigned char n)
{  
	unsigned char i,j,s,tmp,count,index;
	
	count = 0x01;
	s=0x01;
	for(i=0;i<n;i++)
	{
		tmp = *(pSrcBuf+i);    //ת����i��byte
		for(j = 0;j<8;j++)
		{
			if(i<8) 
				index = 2*j; //ÿһ�ֽڵ�8λ��ת���ڲ�ͬ����ʾ��ַ��
			else 
				index=2*j+1;

			if((tmp&count)==count)         
				*(pDstBuf+index) = *(pDstBuf+index) | (s);   //��1      
			else 
				pDstBuf[index] = *(pDstBuf+index) & (~s);    //��0                         
			
			if(count==0x80) 
				count=0x01;   
			else 
				count <<= 1;                                    
		}

		if(s==0x80) 
			s=0x01;     
		else 
			s <<= 1;                  
	}                                
}

void TM1629B_DataConvert(void)
{
	convert1(&TM1629B_buff_1,&TM1629B_buff_2,16);
	// uint8_t i,j,temp;
	// uint8_t mask;

	// mask = 0x80;

	// temp = TM1629B_buff_1[0][0];
	// for (i = 0; i < 8; i++)
	// {
	// 	if (temp & 0x80)
	// 	{
	// 		TM1629B_buff_2[i][0] |= 0x80;
	// 	}
	// 	else
	// 	{
	// 		/* code */
	// 	}
	// 	temp <<= 1;
		
		
	// }
	


	// for (j = 0; j < count; j++)
	// {
	// 	for (i = 0; i < 8; i++)
	// 	{
	// 		TM1629B_buff_2[i][j] = TM1629B_1_buff[j][i];
	// 	}
	// }

}





















