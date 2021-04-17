/**
  ******************************************************************************
  * @file    esp8266.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   esp8266���ܺ���
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "esp8266.h"



static void ESP8266_GPIO_Config(void);
static void ESP8266_USART_Config(void);
void ESP8266_HAL_UART_MspInit(UART_HandleTypeDef *huart);



struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
struct  STRUCT_USARTx_Fram strUSART_Fram_Record = { 0 };

UART_HandleTypeDef ESP8266_UartHandle;


/**
  * @brief  ESP8266��ʼ������
  * @param  ��
  * @retval ��
  */
void ESP8266_Init(void)
{
	ESP8266_GPIO_Config(); 
	
	ESP8266_USART_Config(); 
	
	
	macESP8266_RST_HIGH_LEVEL();

	macESP8266_CH_ENABLE();
	
	
}


/**
  * @brief  ��ʼ��ESP8266�õ���GPIO����
  * @param  ��
  * @retval ��
  */
static void ESP8266_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStruct;

	macESP8266_CH_PD_CLK_ENABLE();
	macESP8266_RST_CLK_ENABLE();
									   
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
    GPIO_InitStruct.Pin = macESP8266_CH_PD_PIN;	
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    /*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    HAL_GPIO_Init(macESP8266_CH_PD_PORT, &GPIO_InitStruct);	


     /*ѡ��Ҫ���Ƶ�GPIO����*/															   
    GPIO_InitStruct.Pin = macESP8266_RST_PIN;	

    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    HAL_GPIO_Init(macESP8266_RST_PORT, &GPIO_InitStruct);	


}


 /**
  * @brief  ESP8266_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */  
void ESP8266_USART_Config(void)
{ 
  ESP8266_HAL_UART_MspInit(&ESP8266_UartHandle);

  ESP8266_UartHandle.Instance          = ESP8266_USART;
  
  ESP8266_UartHandle.Init.BaudRate     = ESP8266_USART_BAUDRATE;
  ESP8266_UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  ESP8266_UartHandle.Init.StopBits     = UART_STOPBITS_1;
  ESP8266_UartHandle.Init.Parity       = UART_PARITY_NONE;
  ESP8266_UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  ESP8266_UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&ESP8266_UartHandle);
    
 /*ʹ�ܴ��ڽ��ն� */
  __HAL_UART_ENABLE_IT(&ESP8266_UartHandle,UART_IT_RXNE);  
  __HAL_UART_ENABLE_IT(&ESP8266_UartHandle,UART_IT_IDLE);  

}


/**
  * @brief UART MSP ��ʼ�� 
  * @param huart: UART handle
  * @retval ��
  */
void ESP8266_HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  ESP8266_USART_CLK_ENABLE();
	
	ESP8266_USART_RX_GPIO_CLK_ENABLE();
  ESP8266_USART_TX_GPIO_CLK_ENABLE();
  
/**USART1 GPIO Configuration    
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX 
  */
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStruct.Pin = ESP8266_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ESP8266_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStruct.Pin = ESP8266_USART_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;	//ģʽҪ����Ϊ��������ģʽ��	
  HAL_GPIO_Init(ESP8266_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
 
  HAL_NVIC_SetPriority(ESP8266_USART_IRQ ,0,3);	//��ռ���ȼ�0�������ȼ�1
  HAL_NVIC_EnableIRQ(ESP8266_USART_IRQ );		    //ʹ��USARTX1�ж�ͨ��  
}


/*
 * ��������ESP8266_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ���� ESP8266_AT_Test ����
 */
void ESP8266_Rst(void)
{
	#if 0
	 ESP8266_Cmd("AT+RST", "OK", "ready", 2500);   	
	
	#else
	 macESP8266_RST_LOW_LEVEL();
	 Delay_ms(500); 
	 macESP8266_RST_HIGH_LEVEL();
	#endif

}


/*
 * ��������ESP8266_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Cmd(char * cmd, char * reply1, char * reply2, uint32_t waittime)
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	macESP8266_Usart("%s\r\n", cmd);
	// USART_printf(&ESP8266_UartHandle, "%s\r\n", cmd);

	if((reply1 == 0) &&(reply2 == 0))    //����Ҫ��������
		return true;
	
	Delay_ms(waittime); //��ʱ
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	macPC_Usart("%s", strEsp8266_Fram_Record .Data_RX_BUF);
  
	if((reply1 != 0) &&(reply2 != 0))
		return((bool) strstr(strEsp8266_Fram_Record .Data_RX_BUF, reply1) || 
						(bool) strstr(strEsp8266_Fram_Record .Data_RX_BUF, reply2)); 
 	
	else if(reply1 != 0)
		return((bool) strstr(strEsp8266_Fram_Record .Data_RX_BUF, reply1));
	
	else
		return((bool) strstr(strEsp8266_Fram_Record .Data_RX_BUF, reply2));
	
}


/*
 * ��������ESP8266_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_AT_Test(void)
{
	char count=0;
	
	macESP8266_RST_HIGH_LEVEL();	
	Delay_ms(1000);
	while(count < 10)
	{
		if(ESP8266_Cmd("AT", "OK", NULL, 500)) return;
		ESP8266_Rst();
		++ count;
	}
}


/*
 * ��������ESP8266_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)
{
	switch(enumMode)
	{
		case STA:
			return ESP8266_Cmd("AT+CWMODE=1", "OK", "no change", 2500); 
		
	  case AP:
		  return ESP8266_Cmd("AT+CWMODE=2", "OK", "no change", 2500); 
		
		case STA_AP:
		  return ESP8266_Cmd("AT+CWMODE=3", "OK", "no change", 2500); 
		
	  default:
		  return false;
  }
	
}


/*
 * ��������ESP8266_JoinAP
 * ����  ��WF-ESP8266ģ�������ⲿWiFi
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_JoinAP(char * pSSID, char * pPassWord)
{
	char cCmd [120];

	sprintf(cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord);
	
	return ESP8266_Cmd(cCmd, "OK", NULL, 5000);
	
}


/*
 * ��������ESP8266_BuildAP
 * ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 *       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_BuildAP(char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode)
{
	char cCmd [120];

	sprintf(cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode);
	
	return ESP8266_Cmd(cCmd, "OK", 0, 1000);
	
}


/*
 * ��������ESP8266_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Enable_MultipleId(FunctionalState enumEnUnvarnishTx)
{
	char cStr [20];
	
	sprintf(cStr, "AT+CIPMUX=%d",(enumEnUnvarnishTx ? 1 : 0));
	
	return ESP8266_Cmd(cStr, "OK", 0, 500);
	
}


/*
 * ��������ESP8266_Link_Server
 * ����  ��WF-ESP8266ģ�������ⲿ������
 * ����  ��enumE������Э��
 *       ��ip��������IP�ַ���
 *       ��ComNum���������˿��ַ���
 *       ��id��ģ�����ӷ�������ID
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch( enumE)
  {
		case enumTCP:
		  sprintf(cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum);
		  break;
		
		case enumUDP:
		  sprintf(cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum);
		  break;
		
		default:
			break;
  }

  if(id < 5)
    sprintf(cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf(cCmd, "AT+CIPSTART=%s", cStr);

	return ESP8266_Cmd(cCmd, "OK", "ALREAY CONNECT", 4000);
	
}


/*
 * ��������ESP8266_StartOrShutServer
 * ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��enumMode������/�ر�
 *       ��pPortNum���������˿ں��ַ���
 *       ��pTimeOver����������ʱʱ���ַ�������λ����
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_StartOrShutServer(FunctionalState enumMode, char * pPortNum, char * pTimeOver)
{
	char cCmd1 [120], cCmd2 [120];

	if(enumMode)
	{
		sprintf(cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum);
		
		sprintf(cCmd2, "AT+CIPSTO=%s", pTimeOver);

		return(ESP8266_Cmd(cCmd1, "OK", 0, 500) &&
						 ESP8266_Cmd(cCmd2, "OK", 0, 500));
	}
	
	else
	{
		sprintf(cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum);

		return ESP8266_Cmd(cCmd1, "OK", 0, 500);
	}
	
}


/*
 * ��������ESP8266_Get_LinkStatus
 * ����  ����ȡ WF-ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
 * ����  ����
 * ����  : 2�����ip
 *         3����������
 *         3��ʧȥ����
 *         0����ȡ״̬ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Get_LinkStatus(void)
{
	if(ESP8266_Cmd("AT+CIPSTATUS", "OK", 0, 500))
	{
		if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n"))
			return 2;
		
		else if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n"))
			return 3;
		
		else if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n"))
			return 4;		

	}
	
	return 0;
	
}


/*
 * ��������ESP8266_Get_IdLinkStatus
 * ����  ����ȡ WF-ESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
 * ����  ����
 * ����  : �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Get_IdLinkStatus(void)
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if(ESP8266_Cmd("AT+CIPSTATUS", "OK", 0, 500))
	{
		if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0,"))
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1,"))
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2,"))
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3,"))
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4,"))
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	

	}
	
	return ucIdLinkStatus;
	
}


/*
 * ��������ESP8266_Inquire_ApIp
 * ����  ����ȡ F-ESP8266 �� AP IP
 * ����  ��pApIp����� AP IP ��������׵�ַ
 *         ucArrayLength����� AP IP ������ĳ���
 * ����  : 0����ȡʧ��
 *         1����ȡ�ɹ�
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Inquire_ApIp(char * pApIp, uint8_t ucArrayLength)
{
	char uc;
	
	char * pCh;
	
	
  ESP8266_Cmd("AT+CIFSR", "OK", 0, 500);
	
	pCh = strstr(strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"");
	
	if(pCh)
		pCh += 6;
	
	else
		return 0;
	
	for(uc = 0; uc < ucArrayLength; uc ++)
	{
		pApIp [ uc ] = *(pCh + uc);
		
		if(pApIp [ uc ] == '\"')
		{
			pApIp [ uc ] = '\0';
			break;
		}
		
	}
	
	return 1;
	
}


/*
 * ��������ESP8266_UnvarnishSend
 * ����  ������WF-ESP8266ģ�����͸������
 * ����  ����
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_UnvarnishSend(void)
{
	if(! ESP8266_Cmd("AT+CIPMODE=1", "OK", 0, 500))
		return false;
	
	return 
	  ESP8266_Cmd("AT+CIPSEND", "OK", ">", 500);
	
}


/*
 * ��������ESP8266_ExitUnvarnishSend
 * ����  ������WF-ESP8266ģ���˳�͸��ģʽ
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_ExitUnvarnishSend(void)
{
	Delay_ms(1000);
	
	macESP8266_Usart("+++");
	
	Delay_ms(500); 
	
}


/*
 * ��������ESP8266_SendString
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 *       ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 *       ��ucId���ĸ�ID���͵��ַ���
 * ����  : 1�����ͳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId)
{
	char cStr [20];
	bool bRet = false;
	
		
	if(enumEnUnvarnishTx)
	{
		macESP8266_Usart("%s", pStr);
		
		bRet = true;
		
	}

	else
	{
		if(ucId < 5)
			sprintf(cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2);

		else
			sprintf(cStr, "AT+CIPSEND=%d", ulStrLength + 2);
		
		ESP8266_Cmd(cStr, "> ", 0, 1000);

		bRet = ESP8266_Cmd(pStr, "SEND OK", 0, 1000);
  }
	
	return bRet;

}


/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
char * ESP8266_ReceiveString(FunctionalState enumEnUnvarnishTx)
{
	char * pRecStr = 0;
	
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while(! strEsp8266_Fram_Record .InfBit .FramFinishFlag);
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if(enumEnUnvarnishTx)
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	
	else 
	{
		if(strstr(strEsp8266_Fram_Record .Data_RX_BUF, "+IPD"))
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}

	return pRecStr;
	
}
