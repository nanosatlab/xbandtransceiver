/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l4xx_hal.h"

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
#define mcu_watchdog_Pin GPIO_PIN_14
#define mcu_watchdog_GPIO_Port GPIOC
#define Tx_Temp_Pin GPIO_PIN_2
#define Tx_Temp_GPIO_Port GPIOA
#define Tx_PM_Pin GPIO_PIN_3
#define Tx_PM_GPIO_Port GPIOA
#define Rx_CSB_Pin GPIO_PIN_4
#define Rx_CSB_GPIO_Port GPIOA
#define TCXO_NE_Pin GPIO_PIN_5
#define TCXO_NE_GPIO_Port GPIOA
#define Tx_CSB_Pin GPIO_PIN_0
#define Tx_CSB_GPIO_Port GPIOB
#define Tx_MUXout_Pin GPIO_PIN_4
#define Tx_MUXout_GPIO_Port GPIOB
#define Rx_CE_Pin GPIO_PIN_5
#define Rx_CE_GPIO_Port GPIOB
#define Rx_MUXout_Pin GPIO_PIN_6
#define Rx_MUXout_GPIO_Port GPIOB
#define Tx_CE_Pin GPIO_PIN_7
#define Tx_CE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
