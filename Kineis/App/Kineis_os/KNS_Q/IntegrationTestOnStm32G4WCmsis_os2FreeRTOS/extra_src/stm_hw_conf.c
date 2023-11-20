/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "stm_hw_conf.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mgr_log.h"

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
// CORDIC_HandleTypeDef hcordic;
//
//DAC_HandleTypeDef hdac1;
//
//FMAC_HandleTypeDef hfmac;
//
//I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

//RTC_HandleTypeDef hrtc;
//
//SAI_HandleTypeDef hsai_BlockA1;
//SAI_HandleTypeDef hsai_BlockB1;
//DMA_HandleTypeDef hdma_sai1_a;
//DMA_HandleTypeDef hdma_sai1_b;
//
//SPI_HandleTypeDef hspi2;
//
//PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_DMA_Init(void);
//static void MX_CORDIC_Init(void);
//static void MX_DAC1_Init(void);
//static void MX_FMAC_Init(void);
//static void MX_I2C1_Init(void);
static void MX_LPUART1_UART_Init(void);
//static void MX_RTC_Init(void);
//static void MX_SAI1_Init(void);
//static void MX_USB_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
void stm_hw_conf(void)
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
//  MX_DMA_Init();
//  MX_CORDIC_Init();
//  MX_DAC1_Init();
//  MX_FMAC_Init();
//  MX_I2C1_Init();
  MX_LPUART1_UART_Init();
//  MX_RTC_Init();
//  MX_SAI1_Init();
//  MX_SPI2_Init();
//  MX_USB_PCD_Init();
  /* USER CODE BEGIN 2 */
  MGR_LOG_DEBUG("==== STM HW configured ====\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* ================================================================== */
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

///**
//  * @brief CORDIC Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_CORDIC_Init(void)
//{
//
//  /* USER CODE BEGIN CORDIC_Init 0 */
//
//  /* USER CODE END CORDIC_Init 0 */
//
//  /* USER CODE BEGIN CORDIC_Init 1 */
//
//  /* USER CODE END CORDIC_Init 1 */
//  hcordic.Instance = CORDIC;
//  if (HAL_CORDIC_Init(&hcordic) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN CORDIC_Init 2 */
//
//  /* USER CODE END CORDIC_Init 2 */
//
//}
//
///**
//  * @brief DAC1 Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_DAC1_Init(void)
//{
//
//  /* USER CODE BEGIN DAC1_Init 0 */
//
//  /* USER CODE END DAC1_Init 0 */
//
//  DAC_ChannelConfTypeDef sConfig = {0};
//
//  /* USER CODE BEGIN DAC1_Init 1 */
//
//  /* USER CODE END DAC1_Init 1 */
//
//  /** DAC Initialization
//  */
//  hdac1.Instance = DAC1;
//  if (HAL_DAC_Init(&hdac1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** DAC channel OUT1 config
//  */
//  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
//  sConfig.DAC_DMADoubleDataMode = DISABLE;
//  sConfig.DAC_SignedFormat = DISABLE;
//  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
//  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
//  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
//  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
//  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
//  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
//  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** DAC channel OUT2 config
//  */
//  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN DAC1_Init 2 */
//
//  /* USER CODE END DAC1_Init 2 */
//
//}
//
///**
//  * @brief FMAC Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_FMAC_Init(void)
//{
//
//  /* USER CODE BEGIN FMAC_Init 0 */
//
//  /* USER CODE END FMAC_Init 0 */
//
//  /* USER CODE BEGIN FMAC_Init 1 */
//
//  /* USER CODE END FMAC_Init 1 */
//  hfmac.Instance = FMAC;
//  if (HAL_FMAC_Init(&hfmac) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN FMAC_Init 2 */
//
//  /* USER CODE END FMAC_Init 2 */
//
//}
//
///**
//  * @brief I2C1 Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_I2C1_Init(void)
//{
//
//  /* USER CODE BEGIN I2C1_Init 0 */
//
//  /* USER CODE END I2C1_Init 0 */
//
//  /* USER CODE BEGIN I2C1_Init 1 */
//
//  /* USER CODE END I2C1_Init 1 */
//  hi2c1.Instance = I2C1;
//  hi2c1.Init.Timing = 0x10A0A6FB;
//  hi2c1.Init.OwnAddress1 = 0;
//  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Configure Analogue filter
//  */
//  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Configure Digital filter
//  */
//  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN I2C1_Init 2 */
//
//  /* USER CODE END I2C1_Init 2 */
//
//}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  huart2.Instance = LPUART1;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
// was not automatically generated by Cube IDE
//  huart2.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

///**
//  * @brief RTC Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_RTC_Init(void)
//{
//
//  /* USER CODE BEGIN RTC_Init 0 */
//
//  /* USER CODE END RTC_Init 0 */
//
//  RTC_TimeTypeDef sTime = {0};
//  RTC_DateTypeDef sDate = {0};
//
//  /* USER CODE BEGIN RTC_Init 1 */
//
//  /* USER CODE END RTC_Init 1 */
//
//  /** Initialize RTC Only
//  */
//  hrtc.Instance = RTC;
//  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
//  hrtc.Init.AsynchPrediv = 127;
//  hrtc.Init.SynchPrediv = 255;
//  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
//  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
//  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
//  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
//  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
//  if (HAL_RTC_Init(&hrtc) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /* USER CODE BEGIN Check_RTC_BKUP */
//
//  /* USER CODE END Check_RTC_BKUP */
//
//  /** Initialize RTC and set the Time and Date
//  */
//  sTime.Hours = 0x0;
//  sTime.Minutes = 0x0;
//  sTime.Seconds = 0x0;
//  sTime.SubSeconds = 0x0;
//  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
//  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
//  sDate.Month = RTC_MONTH_JANUARY;
//  sDate.Date = 0x1;
//  sDate.Year = 0x0;
//
//  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN RTC_Init 2 */
//
//  /* USER CODE END RTC_Init 2 */
//
//}
//
///**
//  * @brief SAI1 Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_SAI1_Init(void)
//{
//
//  /* USER CODE BEGIN SAI1_Init 0 */
//
//  /* USER CODE END SAI1_Init 0 */
//
//  /* USER CODE BEGIN SAI1_Init 1 */
//
//  /* USER CODE END SAI1_Init 1 */
//  hsai_BlockA1.Instance = SAI1_Block_A;
//  hsai_BlockA1.Init.Protocol = SAI_FREE_PROTOCOL;
//  hsai_BlockA1.Init.AudioMode = SAI_MODESLAVE_RX;
//  hsai_BlockA1.Init.DataSize = SAI_DATASIZE_16;
//  hsai_BlockA1.Init.FirstBit = SAI_FIRSTBIT_MSB;
//  hsai_BlockA1.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
//  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
//  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
//  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF;
//  hsai_BlockA1.Init.MckOutput = SAI_MCK_OUTPUT_ENABLE;
//  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
//  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
//  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
//  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
//  hsai_BlockA1.Init.PdmInit.Activation = DISABLE;
//  hsai_BlockA1.Init.PdmInit.MicPairsNbr = 0;
//  hsai_BlockA1.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
//  hsai_BlockA1.FrameInit.FrameLength = 32;
//  hsai_BlockA1.FrameInit.ActiveFrameLength = 1;
//  hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
//  hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
//  hsai_BlockA1.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;
//  hsai_BlockA1.SlotInit.FirstBitOffset = 0;
//  hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
//  hsai_BlockA1.SlotInit.SlotNumber = 2;
//  hsai_BlockA1.SlotInit.SlotActive = 0x0000FFFF;
//  if (HAL_SAI_Init(&hsai_BlockA1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  hsai_BlockB1.Instance = SAI1_Block_B;
//  hsai_BlockB1.Init.Protocol = SAI_FREE_PROTOCOL;
//  hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_TX;
//  hsai_BlockB1.Init.DataSize = SAI_DATASIZE_16;
//  hsai_BlockB1.Init.FirstBit = SAI_FIRSTBIT_MSB;
//  hsai_BlockB1.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
//  hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS;
//  hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
//  hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_3QF;
//  hsai_BlockB1.Init.MckOutput = SAI_MCK_OUTPUT_ENABLE;
//  hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
//  hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
//  hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
//  hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
//  hsai_BlockB1.Init.PdmInit.Activation = DISABLE;
//  hsai_BlockB1.Init.PdmInit.MicPairsNbr = 0;
//  hsai_BlockB1.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
//  hsai_BlockB1.FrameInit.FrameLength = 32;
//  hsai_BlockB1.FrameInit.ActiveFrameLength = 1;
//  hsai_BlockB1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
//  hsai_BlockB1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
//  hsai_BlockB1.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;
//  hsai_BlockB1.SlotInit.FirstBitOffset = 0;
//  hsai_BlockB1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
//  hsai_BlockB1.SlotInit.SlotNumber = 2;
//  hsai_BlockB1.SlotInit.SlotActive = 0x0000FFFF;
//  if (HAL_SAI_Init(&hsai_BlockB1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN SAI1_Init 2 */
//
//  /* USER CODE END SAI1_Init 2 */
//
//}
//
///**
//  * @brief SPI2 Initialization Function
//  * @param None
//  * @retval None
//  */
//void MX_SPI2_Init(void)
//{
//
//  /* USER CODE BEGIN SPI2_Init 0 */
//
//  /* USER CODE END SPI2_Init 0 */
//
//  /* USER CODE BEGIN SPI2_Init 1 */
//
//  /* USER CODE END SPI2_Init 1 */
//  /* SPI2 parameter configuration*/
//  hspi2.Instance = SPI2;
//  hspi2.Init.Mode = SPI_MODE_MASTER;
//  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
//  hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
//  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
//  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
//  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
//  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
//  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
//  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  hspi2.Init.CRCPolynomial = 7;
//  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
//  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
//  if (HAL_SPI_Init(&hspi2) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN SPI2_Init 2 */
//
//  /* USER CODE END SPI2_Init 2 */
//
//}
//
///**
//  * @brief USB Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_USB_PCD_Init(void)
//{
//
//  /* USER CODE BEGIN USB_Init 0 */
//
//  /* USER CODE END USB_Init 0 */
//
//  /* USER CODE BEGIN USB_Init 1 */
//
//  /* USER CODE END USB_Init 1 */
//  hpcd_USB_FS.Instance = USB;
//  hpcd_USB_FS.Init.dev_endpoints = 8;
//  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
//  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
//  hpcd_USB_FS.Init.Sof_enable = DISABLE;
//  hpcd_USB_FS.Init.low_power_enable = DISABLE;
//  hpcd_USB_FS.Init.lpm_enable = DISABLE;
//  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
//  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN USB_Init 2 */
//
//  /* USER CODE END USB_Init 2 */
//
//}
//
///**
//  * Enable DMA controller clock
//  */
//static void MX_DMA_Init(void)
//{
//
//  /* DMA controller clock enable */
//  __HAL_RCC_DMAMUX1_CLK_ENABLE();
//  __HAL_RCC_DMA1_CLK_ENABLE();
//
//  /* DMA interrupt init */
//  /* DMA1_Channel1_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
//  /* DMA1_Channel2_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
//
//}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, IO_SX_EN_Pin|IO_USR_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, IO_LNA_EN_Pin|IO_RFSW_RX_Pin|IO_SX_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, IO_RFSW_TX_Pin|IO_PA_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : EXT_WKUP_BUTTON_Pin */
  GPIO_InitStruct.Pin = EXT_WKUP_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EXT_WKUP_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PG10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IO_SX_EN_Pin IO_USR_2_Pin */
  GPIO_InitStruct.Pin = IO_SX_EN_Pin|IO_USR_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : IO_USR_3_Pin */
  GPIO_InitStruct.Pin = IO_USR_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IO_USR_3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IO_USR_4_Pin IO_SX_PLL_RDY_Pin IO_USR_5_Pin */
  GPIO_InitStruct.Pin = IO_USR_4_Pin|IO_SX_PLL_RDY_Pin|IO_USR_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : IO_LNA_EN_Pin IO_RFSW_RX_Pin IO_SX_RESET_Pin */
  GPIO_InitStruct.Pin = IO_LNA_EN_Pin|IO_RFSW_RX_Pin|IO_SX_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : IO_RFSW_TX_Pin IO_PA_EN_Pin */
  GPIO_InitStruct.Pin = IO_RFSW_TX_Pin|IO_PA_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB6 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
__attribute__((weak)) void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  MGR_LOG_DEBUG("Error_Handler\r\n");
  __disable_irq();
  while (1)
  {
  }
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
__attribute__((weak)) void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  MGR_LOG_DEBUG("ASSERT FAIL: %ld %s\r\n", line, file);
  Error_Handler();
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
