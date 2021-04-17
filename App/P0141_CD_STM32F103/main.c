/*
******************************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2013-xx-xx
* @brief   ����led
******************************************************************************
* @attention
*
* ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
* ��̳    :http://www.firebbs.cn
* �Ա�    :https://fire-stm32.taobao.com
*
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "modules.h"

void SystemClock_Config(void);
void Error_Handler(char *file, uint32_t line);

/********************************** �û���Ҫ���õĲ���**********************************/
#define      macUser_ESP8266_ApSsid                       "2F"                //Ҫ���ӵ��ȵ������
#define      macUser_ESP8266_ApPwd                        "chuandong2F"           //Ҫ���ӵ��ȵ����Կ

#define      macUser_ESP8266_TcpServer_IP                 "192.168.2.117"      //Ҫ���ӵķ������� IP
#define      macUser_ESP8266_TcpServer_Port               "8080"               //Ҫ���ӵķ������Ķ˿�


volatile uint8_t ucTcpClosedFlag = 0;

/********************************** ���Ժ������� ***************************************/
void ESP8266_StaTcpClient_UnvarnishTest(void);

/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;

  	printf ( "\r\n�������� ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );

  	while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n���� ESP8266 ���\r\n" );
	printf ( "\r\n��ʼ͸��......\r\n" );
	
	while ( 1 )
	{
	
		ESP8266_SendString ( ENABLE, "test", 0, Single_ID_0 );               //���� DHT11 ��ʪ����Ϣ�������������
		
		Delay_ms (1000);
		
		if (ucTcpClosedFlag)                                             //����Ƿ�ʧȥ����
		{
			ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
			{
				printf ( "\r\n���������ȵ�ͷ����� ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
		}
	}	
}


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{ 
	
	HAL_Init();        
	/* ����ϵͳʱ��Ϊ72MHz */ 
	SystemClock_Config();

	SysTick_Init();

	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	DEBUG_USART_Config();
  
	ESP8266_Init ();          //��ʼ��WiFiģ��ʹ�õĽӿں�����

	/*����printf��������Ϊ�ض�����fputc��printf�����ݻ����������*/
	printf("��ӭʹ��Ұ�𿪷���\n");	

	/*�Զ��庯����ʽ*/
	Usart_SendString((uint8_t *)"�Զ��庯�����������һ�������жϽ��ջ���ʵ��\n" );

	macESP8266_Usart("U3\r\n"); 

	ESP8266_StaTcpClient_UnvarnishTest();
	
	// while(1)
	// {
	// 	if(strUSART_Fram_Record.InfBit.FramFinishFlag == 1)  //������յ��˴��ڵ������ֵ�����
	// 	{
	// 		strUSART_Fram_Record.Data_RX_BUF[strUSART_Fram_Record.InfBit.FramLength] = '\0';
	// 		macESP8266_Usart(strUSART_Fram_Record.Data_RX_BUF);      //���ݴӴ��ڵ�������ת����ESP8266
	// 		// Usart_SendString((uint8_t *)strUSART_Fram_Record.Data_RX_BUF);        //���ݴ�ESP8266ת�������ڵ�������

	// 		strUSART_Fram_Record.InfBit.FramLength = 0;                                //�������ݳ�������
	// 		strUSART_Fram_Record.InfBit.FramFinishFlag = 0;                            //���ձ�־����
	//   	}
	// 	if(strEsp8266_Fram_Record.InfBit.FramFinishFlag)                             //������յ���ESP8266������
	// 	{                                                      
	// 		strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
	// 		Usart_SendString((uint8_t *)strEsp8266_Fram_Record.Data_RX_BUF);        //���ݴ�ESP8266ת�������ڵ�������

	// 		strEsp8266_Fram_Record.InfBit.FramLength = 0;                             //�������ݳ�������
	// 		strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;                           //���ձ�־����
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
*	�� �� ��: Error_Handler
*	��    ��: file : Դ�����ļ����ơ��ؼ��� __FILE__ ��ʾԴ�����ļ�����
*			  line �������кš��ؼ��� __LINE__ ��ʾԴ�����к�
*	�� �� ֵ: ��
*		Error_Handler(__FILE__, __LINE__);
*********************************************************************************************************
*/
void Error_Handler(char *file, uint32_t line)
{
	/* 
		�û���������Լ��Ĵ��뱨��Դ�����ļ����ʹ����кţ����罫�����ļ����кŴ�ӡ������
		printf("Wrong parameters value: file %s on line %d\r\n", file, line) 
	*/
	
	/* ����һ����ѭ��������ʧ��ʱ������ڴ˴��������Ա����û���� */
	if (line == 0)
	{
		return;
	}
	
	while(1)
	{
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
