/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "subghz.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mcu_at_console.h"
#include "build_info.h"

/** Uncomment below if you want to use Kineis standalone APP or GUI APP.
 * GUI APP is about AT commands support sent to device from GUI or SERIAL hyperterminal).
 * @note both flags are exclusive as standalone app will send periodic frame indefinitively
 */
//#define USE_STDALONE_APP
#define USE_GUI_APP

#ifdef USE_GUI_APP
#include "mgr_at_cmd.h"
#endif
#include "kns_app.h"
#include "mgr_log.h"
#include "kns_q_conf.h"
#include "kns_q.h"
#include "kns_os.h"
#include "kns_mac.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_I2C1_Init();
  MX_SUBGHZ_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

  MGR_LOG_DEBUG("Running build %s done at %s %s\r\n", uc_fw_vers_commit_id, __DATE__, __TIME__);

  assert_param(KNS_Q_create(KNS_Q_DL_APP2MAC, KNS_Q_DL_APP2MAC_LEN,
                 KNS_Q_DL_APP2MAC_ITEM_BYTESIZE) == KNS_STATUS_OK);
  assert_param(KNS_Q_create(KNS_Q_UL_SRVC2MAC, KNS_Q_UL_SRVC2MAC_LEN,
                 KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE) == KNS_STATUS_OK);
  assert_param(KNS_OS_registerTask(KNS_OS_TASK_MAC, KNS_MAC_task) == KNS_STATUS_OK);

#if defined(USE_STDALONE_APP)
  assert_param(KNS_OS_registerTask(KNS_OS_TASK_APP, KNS_APP_stdalone) == KNS_STATUS_OK);
  //assert_param(KNS_OS_registerTask(KNS_OS_TASK_APP, KNS_APP_stdalone_stressTest) == KNS_STATUS_OK);
#elif defined (USE_GUI_APP)
  MGR_AT_CMD_start(&hlpuart1);
  MCU_AT_CONSOLE_send("+FW=%s,%s_%s\r\n", uc_fw_vers_commit_id, __DATE__, __TIME__);
  assert_param(KNS_OS_registerTask(KNS_OS_TASK_APP, KNS_APP_gui) == KNS_STATUS_OK);
#endif

  /** Initialize Kineis MAC profile */
  assert_param(KNS_MAC_init(KNS_MAC_PRFL_BASIC, NULL) == KNS_STATUS_OK);

  KNS_OS_main();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Error_Handler();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS_PWR;
  RCC_OscInitStruct.HSEDiv = RCC_HSE_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  MGR_LOG_DEBUG("Error_Handler\r\n");
#ifdef DEBUG
#ifdef USE_GUI_APP
  MCU_AT_CONSOLE_send("+ASSERT=\r\n");
  HAL_Delay(500);
#endif
  //__disable_irq(); // keep interrupts available as need for HAL_Delau in blink_ERROR_LED
  while (1)
  {
    //blink_ERROR_LED();
  }
#else // end of DEBUG
#ifdef USE_GUI_APP
  MCU_AT_CONSOLE_send("+RST=\r\n");
  HAL_Delay(500);
#endif
  __disable_irq();
  /* reset uC */
  NVIC_SystemReset();
#endif /* #ifdef DEBUG */
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  MGR_LOG_DEBUG("ASSERT FAIL: %lu %s\r\n", line, file);
  Error_Handler();
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
