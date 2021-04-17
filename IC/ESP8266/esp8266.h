#ifndef __ESP8266_H
#define	 __ESP8266_H



#include "modules.h"
#include <stdbool.h>
#include "Common.h"

#if defined(__CC_ARM  )
#pragma anon_unions
#endif

/******************************** ESP8266 连接引脚定义 ***********************************/

#define macESP8266_CH_PD_CLK_ENABLE()    		__HAL_RCC_GPIOE_CLK_ENABLE()
#define macESP8266_CH_PD_PORT			GPIOE
#define macESP8266_CH_PD_PIN 			GPIO_PIN_2

#define macESP8266_RST_CLK_ENABLE()    		__HAL_RCC_GPIOA_CLK_ENABLE()
#define macESP8266_RST_PORT 			GPIOA
#define macESP8266_RST_PIN  			GPIO_PIN_4



//串口波特率
#define ESP8266_USART_BAUDRATE                    115200

//引脚定义
/*******************************************************/
#define ESP8266_USART                             USART3
#define ESP8266_USART_CLK_ENABLE()                __HAL_RCC_USART3_CLK_ENABLE()

#define RCC_PERIPHCLK_UARTx                      RCC_PERIPHCLK_USART3
#define RCC_UARTxCLKSOURCE_SYSCLK                RCC_USART3CLKSOURCE_SYSCLK

#define ESP8266_USART_RX_GPIO_PORT                GPIOB
#define ESP8266_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define ESP8266_USART_RX_PIN                      GPIO_PIN_11
#define ESP8266_USART_RX_AF                       GPIO_AF7_USART3

#define ESP8266_USART_TX_GPIO_PORT                GPIOB
#define ESP8266_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define ESP8266_USART_TX_PIN                      GPIO_PIN_10
#define ESP8266_USART_TX_AF                       GPIO_AF7_USART3

#define ESP8266_USART_IRQHandler                  USART3_IRQHandler
#define ESP8266_USART_IRQ                 		  USART3_IRQn



/*********************************************** ESP8266 函数宏定义 *******************************************/
#define macESP8266_Usart(fmt, ... )  USART_printf(&ESP8266_UartHandle, fmt, ##__VA_ARGS__ ) 
#define macPC_Usart(fmt, ... )printf(fmt, ##__VA_ARGS__ )
//#define macPC_Usart(fmt, ... )

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态

#define macESP8266_CH_ENABLE()   digitalHi(macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN)
#define macESP8266_CH_DISABLE()  digitalLo(macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN)

#define macESP8266_RST_HIGH_LEVEL()  digitalHi(macESP8266_RST_PORT, macESP8266_RST_PIN)
#define macESP8266_RST_LOW_LEVEL()   digitalLo(macESP8266_RST_PORT, macESP8266_RST_PIN)



/******************************* ESP8266 数据类型定义 ***************************/
typedef enum
{
	STA,
	AP,
	STA_AP 
}ENUM_Net_ModeTypeDef;


typedef enum
{
	enumTCP,
	enumUDP,
}ENUM_NetPro_TypeDef;
	

typedef enum
{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
}ENUM_ID_NO_TypeDef;
	

typedef enum
{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
}ENUM_AP_PsdMode_TypeDef;


/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN 1024 //最大接收缓存字节数

extern struct STRUCT_USARTx_Fram //串口数据帧的处理结构体
{
	char Data_RX_BUF [ RX_BUF_MAX_LEN ];
	union 
	{
		__IO uint16_t InfAll;
		struct 
		{
			__IO uint16_t FramLength  :15;  // 14:0 
			__IO uint16_t FramFinishFlag  :1;// 15 
		}InfBit;
	}; 
}strEsp8266_Fram_Record;

extern struct STRUCT_USARTx_Fram strUSART_Fram_Record;


/****************************************** ESP8266 函数声明 ***********************************************/
void ESP8266_Init(void);
void ESP8266_Rst(void);
bool ESP8266_Cmd(char * cmd, char * reply1, char * reply2, uint32_t waittime);
void ESP8266_AT_Test(void);
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
bool ESP8266_JoinAP(char * pSSID, char * pPassWord);
bool ESP8266_BuildAP(char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode);
bool ESP8266_Enable_MultipleId(FunctionalState enumEnUnvarnishTx);
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool ESP8266_StartOrShutServer(FunctionalState enumMode, char * pPortNum, char * pTimeOver);
uint8_t ESP8266_Get_LinkStatus(void);
uint8_t ESP8266_Get_IdLinkStatus(void);
uint8_t ESP8266_Inquire_ApIp(char * pApIp, uint8_t ucArrayLength);
bool ESP8266_UnvarnishSend(void);
void ESP8266_ExitUnvarnishSend(void);
bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId);
char * ESP8266_ReceiveString(FunctionalState enumEnUnvarnishTx);

extern UART_HandleTypeDef ESP8266_UartHandle;


#endif


