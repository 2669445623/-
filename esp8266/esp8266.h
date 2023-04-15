#ifndef __ESP8266_H__
#define __ESP8266_H__

#ifdef __cplusplus
extern "C" {
#endif



#include "stm32f4xx.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "common.h"
#define     macESP8266_RST_HIGH_LEVEL()            GPIO_SetBits ( macESP8266_RST_PORT, macESP8266_RST_PIN )
#pragma anon_unions
//定义结构
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;
/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union 
		{
    __IO uint16_t InfAll;
    struct {
		  __IO uint16_t FramLength       :15;                               // 14:0 
		  __IO uint16_t FramFinishFlag   :1;                                // 15 
	  }InfBit;
  }; 
	
} strEsp8266_Fram_Record;
extern struct STRUCT_USARTx_Fram strUSART_Fram_Record;
/******************************* ESP8266 外部全局变量声明 ***************************/

/*********************************************** ESP8266 函数宏定义 *******************************************/




#define      macESP8266_USART_BAUD_RATE                       115200

#define      macESP8266_USARTx                                USART3
#define      macESP8266_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define      macESP8266_USART_CLK                             RCC_APB1Periph_USART3
#define      macESP8266_USART_GPIO_APBxClock_FUN              RCC_AHB1PeriphClockCmd
#define      macESP8266_USART_GPIO_CLK                        RCC_AHB1Periph_GPIOB   
#define      macESP8266_USART_TX_PORT                         GPIOB   
#define      macESP8266_USART_TX_PIN                          GPIO_Pin_10
#define      macESP8266_USART_TX_AF                           GPIO_AF_USART3
#define      macESP8266_USART_TX_SOURCE                       GPIO_PinSource10

#define      macESP8266_USART_RX_PORT                         GPIOB
#define      macESP8266_USART_RX_PIN                          GPIO_Pin_11
#define      macESP8266_USART_RX_AF                           GPIO_AF_USART3
#define      macESP8266_USART_RX_SOURCE                       GPIO_PinSource11

#define      macESP8266_USART_IRQ                             USART3_IRQn
#define      macESP8266_USART_INT_FUN                         USART3_IRQHandler



/******************************** ESP8266 连接引脚定义 ***********************************/

#define     macESP8266_Usart( fmt, ... )           USART_printf ( macESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
#define     macPC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )
void ESP8266_MODE_AP(void);
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode );
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, uint16_t waittime );
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );
bool ESP8266_SendData (char * pStr, uint32_t ulStrLength);
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
void Select_User_To_Clear(uint8_t fuhao);
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx );
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
void ESP8266_Init ( void );
	#ifdef __cplusplus
}
#endif

#endif /* __ESP8266_H__ */	
