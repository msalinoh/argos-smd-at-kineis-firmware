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
#include "stm32g4xx_hal.h"

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

void stm_hw_conf(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EXT_WKUP_BUTTON_Pin GPIO_PIN_13
#define EXT_WKUP_BUTTON_GPIO_Port GPIOC
#define EXT_WKUP_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define IO_SX_EN_Pin GPIO_PIN_6
#define IO_SX_EN_GPIO_Port GPIOA
#define IO_USR_2_Pin GPIO_PIN_7
#define IO_USR_2_GPIO_Port GPIOA
#define IO_USR_3_Pin GPIO_PIN_4
#define IO_USR_3_GPIO_Port GPIOC
#define IO_USR_4_Pin GPIO_PIN_0
#define IO_USR_4_GPIO_Port GPIOB
#define IO_SX_PLL_RDY_Pin GPIO_PIN_1
#define IO_SX_PLL_RDY_GPIO_Port GPIOB
#define IO_USR_5_Pin GPIO_PIN_2
#define IO_USR_5_GPIO_Port GPIOB
#define IO_LNA_EN_Pin GPIO_PIN_10
#define IO_LNA_EN_GPIO_Port GPIOB
#define IO_RFSW_RX_Pin GPIO_PIN_11
#define IO_RFSW_RX_GPIO_Port GPIOB
#define IO_RFSW_TX_Pin GPIO_PIN_6
#define IO_RFSW_TX_GPIO_Port GPIOC
#define SAI1_FS_A_IO_SX_LO_RDY_Pin GPIO_PIN_9
#define SAI1_FS_A_IO_SX_LO_RDY_GPIO_Port GPIOA
#define IO_PA_EN_Pin GPIO_PIN_10
#define IO_PA_EN_GPIO_Port GPIOC
#define IO_SX_RESET_Pin GPIO_PIN_9
#define IO_SX_RESET_GPIO_Port GPIOB
void   MX_SPI2_Init(void);
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
