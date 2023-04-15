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
  * @brief  ESP8266初始化函数
  * @param  无
  * @retval 无
  */
void ESP8266_Init ( void )
{
	ESP8266_MODE_AP();
	
	osDelay(1);
}

//配置esp8266
void ESP8266_MODE_AP(void)
{
	//注释掉为调试窗口内容
//  printf( "\r\n正在配置 ESP8266 ......\r\n" );
//  printf( "\r\n使能 ESP8266 ......\r\n" );
//	macESP8266_CH_ENABLE();
//  printf( "\r\n正在配置工作模式 AP ......\r\n" );
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
//	ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 ); //重启esp9266

//  printf( "\r\n正在连接 WiFi ......\r\n" );
//  while( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
		
//	while( ! ESP8266_BuildAP("123","12345678",4));	//创建wifi
//  printf( "\r\n开启多连接 ......\r\n" );
//	while( ! ESP8266_Enable_MultipleId ( ENABLE ) );
	
//	printf("\r\n正在建立服务器......\r\n");
//	while (!ESP8266_StartOrShutServer ( ENABLE, "8080", "1000" ));	
	
//	printf( "\r\n配置 ESP8266 完毕\r\n" );
	
	
}






/*
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_BuildAP
 * 描述  ：WF-ESP8266模块创建WiFi热点
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 *       ：enunPsdMode，WiFi加密方式代号字符串
 * 返回  : 1，创建成功
 *         0，创建失败
 * 调用  ：被外部调用
 */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	
}


/*
 * 函数名：ESP8266_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, uint16_t waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包

	macESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	for(int i=0;i<waittime;i++)
	{
		osDelay(1);
	}                //延时
	
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
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
}
/*
 * 函数名：ESP8266_StartOrShutServer
 * 描述  ：WF-ESP8266模块开启或关闭服务器模式
 * 输入  ：enumMode，开启/关闭
 *       ：pPortNum，服务器端口号字符串
 *       ：pTimeOver，服务器超时时间字符串，单位：秒
 * 返回  : 1，操作成功
 *         0，操作失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_SendData
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 * 返回  : 1，发送成功
 *         0，发送失败
 * 调用  ：被外部调用
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
		case 0x30://停止
		{
		
			pid_motor_setting_sudu(0,0,0,0);
			osDelay(1);
			break;
	 }
		case 0x31://前进
		{		
			pid_motor_setting(1000,1000,1000,1000);
			osDelay(1);
			break;
		}
		case 0x32://后退
		{
			
			pid_motor_setting(-1000,-1000,-1000,-1000);
			osDelay(1);
			break;
		}
		case 0x33://左移
		{ 
			pid_motor_setting(-1000,1000,-1000,1000);
			osDelay(1);
			break;
		}
		case 0x34://右移
		{
			pid_motor_setting(1000,-1000,1000,-1000);
			osDelay(1);
			break;
		}
		case 0x35://左上
		{
			pid_motor_setting(0,1000,0,1000);
			osDelay(1);
			break;
		}
		case 0x36://右上
		{
			pid_motor_setting(1000,0,1000,0);
			osDelay(1);
			break;
		}
		case 0x37://左下
		{
			pid_motor_setting(-1000,0,-1000,0);
			osDelay(1);
			break;
		}
			case 0x38://右下
		{
			pid_motor_setting(0,-1000,0,-1000);
			osDelay(1);
			break;
		}
		case 0x39://上升
		{
			pid_elevator_setting_sudu_remote(1000);
			osDelay(1);
			break;
		}
			case 0x61://下降
		{
			pid_elevator_setting_sudu_remote(-1000);
			osDelay(1);
			break;
		}
			case 0x62://抓取
			{
				for(int i=0;i<10;i++)
		   {
				 zhuazi(870);
				 osDelay(1);
			 }
			 	break;
		  }
			case 0x63://放置
			{
				 zhuazi(1200);
				 osDelay(1);
			 	 break;
		  }
			
			case 0x64://自主
				moshi=1;
			 	break;
			
			case 0x65:	//遥控	
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
				 pid_yuntai_setting_sudu(10000); //右旋转
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
HAL_UART_Transmit(&huart3, temp, 1, 2);//huart1需要根据你的配置修改
return ch;
}

