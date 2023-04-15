#ifndef __COMMON_H
#define __COMMON_H



#include "stm32f4xx.h"



/******************************* 宏定义 ***************************/
#define            macNVIC_PriorityGroup_x                     NVIC_PriorityGroup_2



/********************************** 函数声明 ***************************************/
void                     USART_printf                       ( USART_TypeDef * USARTx, char * Data, ... );
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);

#endif /* __COMMON_H */

