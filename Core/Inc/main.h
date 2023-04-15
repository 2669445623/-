/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define deng_Pin GPIO_PIN_2
#define deng_GPIO_Port GPIOE
#define openmv_moshi_Pin GPIO_PIN_3
#define openmv_moshi_GPIO_Port GPIOE
#define f_r2_Pin GPIO_PIN_4
#define f_r2_GPIO_Port GPIOA
#define r_r2_Pin GPIO_PIN_5
#define r_r2_GPIO_Port GPIOA
#define f_r1_Pin GPIO_PIN_6
#define f_r1_GPIO_Port GPIOA
#define r_r1_Pin GPIO_PIN_7
#define r_r1_GPIO_Port GPIOA
#define f_m_Pin GPIO_PIN_4
#define f_m_GPIO_Port GPIOC
#define r_m_Pin GPIO_PIN_5
#define r_m_GPIO_Port GPIOC
#define f_l1_Pin GPIO_PIN_0
#define f_l1_GPIO_Port GPIOB
#define r_l1_Pin GPIO_PIN_1
#define r_l1_GPIO_Port GPIOB
#define f_l2_Pin GPIO_PIN_7
#define f_l2_GPIO_Port GPIOE
#define r_l2_Pin GPIO_PIN_8
#define r_l2_GPIO_Port GPIOE
#define l_l2_Pin GPIO_PIN_15
#define l_l2_GPIO_Port GPIOB
#define b_l2_Pin GPIO_PIN_8
#define b_l2_GPIO_Port GPIOD
#define l_l1_Pin GPIO_PIN_9
#define l_l1_GPIO_Port GPIOD
#define b_l1_Pin GPIO_PIN_10
#define b_l1_GPIO_Port GPIOD
#define l_m_Pin GPIO_PIN_11
#define l_m_GPIO_Port GPIOD
#define b_m_Pin GPIO_PIN_12
#define b_m_GPIO_Port GPIOD
#define l_r1_Pin GPIO_PIN_13
#define l_r1_GPIO_Port GPIOD
#define b_r1_Pin GPIO_PIN_14
#define b_r1_GPIO_Port GPIOD
#define l_r2_Pin GPIO_PIN_15
#define l_r2_GPIO_Port GPIOD
#define b_r2_Pin GPIO_PIN_6
#define b_r2_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
