/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId dianjiHandle;
osThreadId duoji_1Handle;
osThreadId jy62Handle;
osThreadId xianshiHandle;
osThreadId esp8266Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void chasis(void const * argument);
void duoji(void const * argument);
void JY62_1(void const * argument);
void xianshi_1(void const * argument);
void ESP8266(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of dianji */
  osThreadDef(dianji, chasis, osPriorityNormal, 0, 4500);
  dianjiHandle = osThreadCreate(osThread(dianji), NULL);

  /* definition and creation of duoji_1 */
  osThreadDef(duoji_1, duoji, osPriorityNormal, 0, 4500);
  duoji_1Handle = osThreadCreate(osThread(duoji_1), NULL);

  /* definition and creation of jy62 */
  osThreadDef(jy62, JY62_1, osPriorityNormal, 0, 128);
  jy62Handle = osThreadCreate(osThread(jy62), NULL);

  /* definition and creation of xianshi */
  osThreadDef(xianshi, xianshi_1, osPriorityNormal, 0, 256);
  xianshiHandle = osThreadCreate(osThread(xianshi), NULL);

  /* definition and creation of esp8266 */
  osThreadDef(esp8266, ESP8266, osPriorityIdle, 0, 128);
  esp8266Handle = osThreadCreate(osThread(esp8266), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_chasis */
/**
  * @brief  Function implementing the duoji thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_chasis */
//void chasis(void const * argument)
//{
//  /* USER CODE BEGIN chasis */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////  /* Infinite loop */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////  for(;;)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////  {
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////    osDelay(1);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////  }
//  /* USER CODE END chasis */
//}

/* USER CODE BEGIN Header_duoji */
/**
* @brief Function implementing the duoji_1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_duoji */
//void duoji(void const * argument)
//{
//  /* USER CODE BEGIN duoji */
////////////////////////////////////////////////  /* Infinite loop */
////////////////////////////////////////////////  for(;;)
////////////////////////////////////////////////  {
////////////////////////////////////////////////    osDelay(1);
////////////////////////////////////////////////  }
//  /* USER CODE END duoji */
//}

/* USER CODE BEGIN Header_JY62_1 */
/**
* @brief Function implementing the jy62 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_JY62_1 */
//void JY62_1(void const * argument)
//{
//  /* USER CODE BEGIN JY62_1 */
////////////////////////////////////////////  /* Infinite loop */
////////////////////////////////////////////  for(;;)
////////////////////////////////////////////  {
////////////////////////////////////////////    osDelay(1);
////////////////////////////////////////////  }
//  /* USER CODE END JY62_1 */
//}

/* USER CODE BEGIN Header_xianshi_1 */
/**
* @brief Function implementing the xianshi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_xianshi_1 */
//void xianshi_1(void const * argument)
//{
//  /* USER CODE BEGIN xianshi_1 */
//////////////////////////////////////////  /* Infinite loop */
//////////////////////////////////////////  for(;;)
//////////////////////////////////////////  {
//////////////////////////////////////////    osDelay(1);
//////////////////////////////////////////  }
//  /* USER CODE END xianshi_1 */
//}

/* USER CODE BEGIN Header_ESP8266 */
/**
* @brief Function implementing the esp8266 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ESP8266 */
//void ESP8266(void const * argument)
//{
//  /* USER CODE BEGIN ESP8266 */
//////////////  /* Infinite loop */
//////////////  for(;;)
//////////////  {
//////////////    osDelay(1);
//////////////  }
//  /* USER CODE END ESP8266 */
//}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
