/**
	******************************************************************************
	* @file    main.c 
	* @author  xxc
	* @version V1.0
	* @date    2021-4-11
	* @Company 
	* @brief   Main program body
	 ******************************************************************************
	* @attention

	******************************************************************************
	*/ 



/* Includes ------------------------------------------------------------------*/



/* Includes ------------------------------------------------------------------*/
#include "modules.h"

void SystemClock_Config(void);
void Error_Handler(char *file, uint32_t line);

 
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	HAL_Init();        
	/* ����ϵͳʱ��Ϊ180 MHz */ 
	SystemClock_Config();
		
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	DEBUG_USART_Config();

	ESP8266_Init ();          //��ʼ��WiFiģ��ʹ�õĽӿں�����
	
	/*����printf��������Ϊ�ض�����fputc��printf�����ݻ����������*/
	printf("��ӭʹ��Ұ�𿪷���\n");	

	/*�Զ��庯����ʽ*/
	Usart_SendString( (uint8_t *)"�Զ��庯�����������һ�������жϽ��ջ���ʵ��\n" );
		
	while(1)
	{
		if(strUSART_Fram_Record .InfBit .FramFinishFlag == 1)  //������յ��˴��ڵ������ֵ�����
		{
			strUSART_Fram_Record .Data_RX_BUF[strUSART_Fram_Record .InfBit .FramLength] = '\0';
			Usart_SendString(macESP8266_USARTx ,strUSART_Fram_Record .Data_RX_BUF);      //���ݴӴ��ڵ�������ת����ESP8266
			strUSART_Fram_Record .InfBit .FramLength = 0;                                //�������ݳ�������
			strUSART_Fram_Record .InfBit .FramFinishFlag = 0;                            //���ձ�־����
	  	}
		if(strEsp8266_Fram_Record .InfBit .FramFinishFlag)                             //������յ���ESP8266������
		{                                                      
			strEsp8266_Fram_Record .Data_RX_BUF[strEsp8266_Fram_Record .InfBit .FramLength] = '\0';
			Usart_SendString(DEBUG_USART ,strEsp8266_Fram_Record .Data_RX_BUF);        //���ݴ�ESP8266ת�������ڵ�������
			strEsp8266_Fram_Record .InfBit .FramLength = 0;                             //�������ݳ�������
			strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;                           //���ձ�־����
		}		
	}
}




/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
 void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	
	/* The voltage scaling allows optimizing the power consumption when the device is 
		clocked below the maximum system frequency, to update the voltage scaling value 
		regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
	
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
		clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
        Error_Handler(__FILE__, __LINE__);
	}

	/* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	if (HAL_GetREVID() == 0x1001)
	{
		/* Enable the Flash prefetch */
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
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
