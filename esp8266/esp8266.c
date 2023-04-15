#include "esp8266.h"
#include <stdbool.h>
#include <stdio.h>  
#include <string.h>    
#include "main.h"
#include "cmsis_os.h"
#include "wheel.h"
#include "duoji.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
struct  STRUCT_USARTx_Fram strUSART_Fram_Record = { 0 };

int moshi=0;
char ap[]="AT+CWMODE=2\r\n\r";
char rst[]="AT+RST\r\n\r";
char BuildAP[]="AT+CWSAP=\"123\",\"12345678\",1,4\r\n\r";
char duomoshi[]="AT+CIPMUX=1\r\n\r";
char ip[]="AT+CIPSERVER=1,8080\r\n\r";
extern uint8_t esp8266date[12];
void ESP8266(void const * argument)
{
  /* USER CODE BEGIN chasis */
  /* Infinite loop */
  for(;;)
  {
		if(moshi!=1)
		{
		Select_User_To_Clear(esp8266date[11]);
 			osDelay(1);
 
		}
	} /* USER CODE END chasis */
}




/**
  * @brief  ESP8266��ʼ������
  * @param  ��
  * @retval ��
  */
void ESP8266_Init ( void )
{
	ESP8266_MODE_AP();
	
	osDelay(1);
}

//����esp8266
void ESP8266_MODE_AP(void)
{
	//ע�͵�Ϊ���Դ�������
//  printf( "\r\n�������� ESP8266 ......\r\n" );
//  printf( "\r\nʹ�� ESP8266 ......\r\n" );
//	macESP8266_CH_ENABLE();
//  printf( "\r\n�������ù���ģʽ AP ......\r\n" );
//	HAL_UART_Transmit(&huart2,(uint8_t *)ap,sizeof(ap),100);
//	osDelay(50);
//  HAL_UART_Transmit(&huart2,(uint8_t *)rst,sizeof(rst),100);
//	osDelay(70);
	HAL_UART_Transmit(&huart3,(uint8_t *)BuildAP,sizeof(BuildAP),100);
	for(int i=0;i<2000;i++)
	{
		osDelay(1);
	}
	HAL_UART_Transmit(&huart3,(uint8_t *)duomoshi,sizeof(duomoshi),100);
	osDelay(50);
	HAL_UART_Transmit(&huart3,(uint8_t *)ip,sizeof(ip),100);
	osDelay(50);
//  HAL_UART_Transmit(&huart2,(uint8_t *)rst,sizeof(rst),100);
//	osDelay(70);
//	while( ! ESP8266_Net_Mode_Choose ( AP ) );
//	ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 ); //����esp9266

//  printf( "\r\n�������� WiFi ......\r\n" );
//  while( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
		
//	while( ! ESP8266_BuildAP("123","12345678",4));	//����wifi
//  printf( "\r\n���������� ......\r\n" );
//	while( ! ESP8266_Enable_MultipleId ( ENABLE ) );
	
//	printf("\r\n���ڽ���������......\r\n");
//	while (!ESP8266_StartOrShutServer ( ENABLE, "8080", "1000" ));	
	
//	printf( "\r\n���� ESP8266 ���\r\n" );
	
	
}






/*
 * ��������ESP8266_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  case AP:
		  return ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
		case STA_AP:
		  return ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  default:
		  return false;
  }
	
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
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	
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
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, uint16_t waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	macESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	for(int i=0;i<waittime;i++)
	{
		osDelay(1);
	}                //��ʱ
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	macPC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

/*
 * ��������ESP8266_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
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
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
	
}


/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}

	return pRecStr;
	
}


/*
 * ��������ESP8266_SendData
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 * ����  : 1�����ͳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_SendData (char * pStr, uint32_t ulStrLength)
{
	char cStr [20];
	bool bRet = false;
	
	sprintf ( cStr, "AT+CIPSEND=%d,%d", 0, ulStrLength + 2 );

	ESP8266_Cmd ( cStr, "> ", 0, 100 );

	bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 500 );
  
	
	return bRet;

}
void Select_User_To_Clear(uint8_t fuhao)
{
	switch (fuhao)
	{	
		case 0x30://ֹͣ
		{
		
			pid_motor_setting_sudu(0,0,0,0);
			osDelay(1);
			break;
	 }
		case 0x31://ǰ��
		{		
			pid_motor_setting(1000,1000,1000,1000);
			osDelay(1);
			break;
		}
		case 0x32://����
		{
			
			pid_motor_setting(-1000,-1000,-1000,-1000);
			osDelay(1);
			break;
		}
		case 0x33://����
		{ 
			pid_motor_setting(-1000,1000,-1000,1000);
			osDelay(1);
			break;
		}
		case 0x34://����
		{
			pid_motor_setting(1000,-1000,1000,-1000);
			osDelay(1);
			break;
		}
		case 0x35://����
		{
			pid_motor_setting(0,1000,0,1000);
			osDelay(1);
			break;
		}
		case 0x36://����
		{
			pid_motor_setting(1000,0,1000,0);
			osDelay(1);
			break;
		}
		case 0x37://����
		{
			pid_motor_setting(-1000,0,-1000,0);
			osDelay(1);
			break;
		}
			case 0x38://����
		{
			pid_motor_setting(0,-1000,0,-1000);
			osDelay(1);
			break;
		}
		case 0x39://����
		{
			pid_elevator_setting_sudu_remote(1000);
			osDelay(1);
			break;
		}
			case 0x61://�½�
		{
			pid_elevator_setting_sudu_remote(-1000);
			osDelay(1);
			break;
		}
			case 0x62://ץȡ
			{
				for(int i=0;i<10;i++)
		   {
				 zhuazi(870);
				 osDelay(1);
			 }
			 	break;
		  }
			case 0x63://����
			{
				 zhuazi(1200);
				 osDelay(1);
			 	 break;
		  }
			
			case 0x64://����
				moshi=1;
			 	break;
			
			case 0x65:	//ң��	
				moshi=2;
			  break;
			
			case 0x66:
			{
				 pid_elevator_setting_sudu(0);
			   osDelay(1);
			 	break;
		  }
			case 0x67:
			{
				 pid_motor_setting(1000,-1000,-1000,1000);
			   osDelay(1);
			 	break;
			}
			
			case 0x68:
			{
				 pid_motor_setting(-1000,1000,1000,-1000);
			   osDelay(1);
			 	break;
			}
			
			case 0x69:
			{				
				 pid_yuntai_setting_sudu(10000); //����ת
			   osDelay(1);
			 	break;
			}
			
			case 0x6A:
			{
         pid_yuntai_setting(4750);
			   osDelay(1);
			  	break;
			}
			
			case 0x6B://
			{
         CAN_anter_chassis(0); 
			   osDelay(1);
			 	break;
			}
			
		  case 0x6c:
			{
				pid_yuntai_setting_sudu(-10000);
				osDelay(1);
				break;
				
			}
			case 0x6d:
			{
				for(int i=0;i<1000;i++)
			 {
				openmv(1600);
				osDelay(1);
			 }
			 break;
			}
			
			case 0x6e:
			{
				for(int i=0;i<100;i++)
			 {
				openmv(900);
				osDelay(1);
			 }
        break;
			}	
			
			default :
			{
				for(int i=0;i<10;i++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
				}
			}
			
	}
}

int fputc(int ch, FILE *f)
	{
uint8_t temp[1] = {ch};
HAL_UART_Transmit(&huart3, temp, 1, 2);//huart1��Ҫ������������޸�
return ch;
}

