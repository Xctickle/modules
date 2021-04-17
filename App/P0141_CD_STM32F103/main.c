/*
******************************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2013-xx-xx
* @brief   测试led
******************************************************************************
* @attention
*
* 实验平台:野火 F103-霸道 STM32 开发板 
* 论坛    :http://www.firebbs.cn
* 淘宝    :https://fire-stm32.taobao.com
*
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "modules.h"

void SystemClock_Config(void);
void Error_Handler(char *file, uint32_t line);

/********************************** 用户需要设置的参数**********************************/
#define      macUser_ESP8266_ApSsid                       "2F"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "chuandong2F"           //要连接的热点的密钥

#define      macUser_ESP8266_TcpServer_IP                 "192.168.2.117"      //要连接的服务器的 IP
#define      macUser_ESP8266_TcpServer_Port               "8080"               //要连接的服务器的端口


volatile uint8_t ucTcpClosedFlag = 0;

/********************************** 测试函数声明 ***************************************/
void ESP8266_StaTcpClient_UnvarnishTest(void);

/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;

  	printf ( "\r\n正在配置 ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );

  	while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n配置 ESP8266 完毕\r\n" );
	printf ( "\r\n开始透传......\r\n" );
	
	while ( 1 )
	{
	
		ESP8266_SendString ( ENABLE, "test", 0, Single_ID_0 );               //发送 DHT11 温湿度信息到网络调试助手
		
		Delay_ms (1000);
		
		if (ucTcpClosedFlag)                                             //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //确认失去连接后重连
			{
				printf ( "\r\n正在重连热点和服务器 ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
		}
	}	
}


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{ 
	
	HAL_Init();        
	/* 配置系统时钟为72MHz */ 
	SystemClock_Config();

	SysTick_Init();

	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	DEBUG_USART_Config();
  
	ESP8266_Init ();          //初始化WiFi模块使用的接口和外设

	/*调用printf函数，因为重定向了fputc，printf的内容会输出到串口*/
	printf("欢迎使用野火开发板\n");	

	/*自定义函数方式*/
	Usart_SendString((uint8_t *)"自定义函数输出：这是一个串口中断接收回显实验\n" );

	macESP8266_Usart("U3\r\n"); 

	ESP8266_StaTcpClient_UnvarnishTest();
	
	// while(1)
	// {
	// 	if(strUSART_Fram_Record.InfBit.FramFinishFlag == 1)  //如果接收到了串口调试助手的数据
	// 	{
	// 		strUSART_Fram_Record.Data_RX_BUF[strUSART_Fram_Record.InfBit.FramLength] = '\0';
	// 		macESP8266_Usart(strUSART_Fram_Record.Data_RX_BUF);      //数据从串口调试助手转发到ESP8266
	// 		// Usart_SendString((uint8_t *)strUSART_Fram_Record.Data_RX_BUF);        //数据从ESP8266转发到串口调试助手

	// 		strUSART_Fram_Record.InfBit.FramLength = 0;                                //接收数据长度置零
	// 		strUSART_Fram_Record.InfBit.FramFinishFlag = 0;                            //接收标志置零
	//   	}
	// 	if(strEsp8266_Fram_Record.InfBit.FramFinishFlag)                             //如果接收到了ESP8266的数据
	// 	{                                                      
	// 		strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
	// 		Usart_SendString((uint8_t *)strEsp8266_Fram_Record.Data_RX_BUF);        //数据从ESP8266转发到串口调试助手

	// 		strEsp8266_Fram_Record.InfBit.FramLength = 0;                             //接收数据长度置零
	// 		strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;                           //接收标志置零
	// 	}		
	// }	
}



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 2
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef clkinitstruct = {0};
	RCC_OscInitTypeDef oscinitstruct = {0};
	
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState        = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
	oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
		clocks dividers */
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
}

/*
*********************************************************************************************************
*	函 数 名: Error_Handler
*	形    参: file : 源代码文件名称。关键字 __FILE__ 表示源代码文件名。
*			  line ：代码行号。关键字 __LINE__ 表示源代码行号
*	返 回 值: 无
*		Error_Handler(__FILE__, __LINE__);
*********************************************************************************************************
*/
void Error_Handler(char *file, uint32_t line)
{
	/* 
		用户可以添加自己的代码报告源代码文件名和代码行号，比如将错误文件和行号打印到串口
		printf("Wrong parameters value: file %s on line %d\r\n", file, line) 
	*/
	
	/* 这是一个死循环，断言失败时程序会在此处死机，以便于用户查错 */
	if (line == 0)
	{
		return;
	}
	
	while(1)
	{
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
