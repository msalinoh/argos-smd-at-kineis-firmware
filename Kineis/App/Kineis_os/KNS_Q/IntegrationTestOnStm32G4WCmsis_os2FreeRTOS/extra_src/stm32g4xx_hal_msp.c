/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32g4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  HAL_PWREx_DisableUCPDDeadBattery();

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

///**
//* @brief CORDIC MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hcordic: CORDIC handle pointer
//* @retval None
//*/
//void HAL_CORDIC_MspInit(CORDIC_HandleTypeDef* hcordic)
//{
//  if(hcordic->Instance==CORDIC)
//  {
//  /* USER CODE BEGIN CORDIC_MspInit 0 */
//
//  /* USER CODE END CORDIC_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_CORDIC_CLK_ENABLE();
//  /* USER CODE BEGIN CORDIC_MspInit 1 */
//
//  /* USER CODE END CORDIC_MspInit 1 */
//  }
//
//}
//
///**
//* @brief CORDIC MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hcordic: CORDIC handle pointer
//* @retval None
//*/
//void HAL_CORDIC_MspDeInit(CORDIC_HandleTypeDef* hcordic)
//{
//  if(hcordic->Instance==CORDIC)
//  {
//  /* USER CODE BEGIN CORDIC_MspDeInit 0 */
//
//  /* USER CODE END CORDIC_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_CORDIC_CLK_DISABLE();
//  /* USER CODE BEGIN CORDIC_MspDeInit 1 */
//
//  /* USER CODE END CORDIC_MspDeInit 1 */
//  }
//
//}
//
///**
//* @brief DAC MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hdac: DAC handle pointer
//* @retval None
//*/
//void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hdac->Instance==DAC1)
//  {
//  /* USER CODE BEGIN DAC1_MspInit 0 */
//
//  /* USER CODE END DAC1_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_DAC1_CLK_ENABLE();
//
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    /**DAC1 GPIO Configuration
//    PA4     ------> DAC1_OUT1
//    PA5     ------> DAC1_OUT2
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
//    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  /* USER CODE BEGIN DAC1_MspInit 1 */
//
//  /* USER CODE END DAC1_MspInit 1 */
//  }
//
//}
//
///**
//* @brief DAC MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hdac: DAC handle pointer
//* @retval None
//*/
//void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
//{
//  if(hdac->Instance==DAC1)
//  {
//  /* USER CODE BEGIN DAC1_MspDeInit 0 */
//
//  /* USER CODE END DAC1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_DAC1_CLK_DISABLE();
//
//    /**DAC1 GPIO Configuration
//    PA4     ------> DAC1_OUT1
//    PA5     ------> DAC1_OUT2
//    */
//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);
//
//  /* USER CODE BEGIN DAC1_MspDeInit 1 */
//
//  /* USER CODE END DAC1_MspDeInit 1 */
//  }
//
//}
//
///**
//* @brief FMAC MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hfmac: FMAC handle pointer
//* @retval None
//*/
//void HAL_FMAC_MspInit(FMAC_HandleTypeDef* hfmac)
//{
//  if(hfmac->Instance==FMAC)
//  {
//  /* USER CODE BEGIN FMAC_MspInit 0 */
//
//  /* USER CODE END FMAC_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_FMAC_CLK_ENABLE();
//  /* USER CODE BEGIN FMAC_MspInit 1 */
//
//  /* USER CODE END FMAC_MspInit 1 */
//  }
//
//}
//
///**
//* @brief FMAC MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hfmac: FMAC handle pointer
//* @retval None
//*/
//void HAL_FMAC_MspDeInit(FMAC_HandleTypeDef* hfmac)
//{
//  if(hfmac->Instance==FMAC)
//  {
//  /* USER CODE BEGIN FMAC_MspDeInit 0 */
//
//  /* USER CODE END FMAC_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_FMAC_CLK_DISABLE();
//  /* USER CODE BEGIN FMAC_MspDeInit 1 */
//
//  /* USER CODE END FMAC_MspDeInit 1 */
//  }
//
//}
//
///**
//* @brief I2C MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hi2c: I2C handle pointer
//* @retval None
//*/
//void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//  if(hi2c->Instance==I2C1)
//  {
//  /* USER CODE BEGIN I2C1_MspInit 0 */
//
//  /* USER CODE END I2C1_MspInit 0 */
//
//  /** Initializes the peripherals clocks
//  */
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
//    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**I2C1 GPIO Configuration
//    PA15     ------> I2C1_SCL
//    PB7     ------> I2C1_SDA
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_15;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_7;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    /* Peripheral clock enable */
//    __HAL_RCC_I2C1_CLK_ENABLE();
//  /* USER CODE BEGIN I2C1_MspInit 1 */
//
//  /* USER CODE END I2C1_MspInit 1 */
//  }
//
//}
//
///**
//* @brief I2C MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hi2c: I2C handle pointer
//* @retval None
//*/
//void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
//{
//  if(hi2c->Instance==I2C1)
//  {
//  /* USER CODE BEGIN I2C1_MspDeInit 0 */
//
//  /* USER CODE END I2C1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_I2C1_CLK_DISABLE();
//
//    /**I2C1 GPIO Configuration
//    PA15     ------> I2C1_SCL
//    PB7     ------> I2C1_SDA
//    */
//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);
//
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
//
//  /* USER CODE BEGIN I2C1_MspDeInit 1 */
//
//  /* USER CODE END I2C1_MspDeInit 1 */
//  }
//
//}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(huart->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspInit 0 */

  /* USER CODE END LPUART1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_LPUART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**LPUART1 GPIO Configuration
    PA2     ------> LPUART1_TX
    PA3     ------> LPUART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF12_LPUART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN LPUART1_MspInit 1 */

  /* USER CODE END LPUART1_MspInit 1 */
  }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspDeInit 0 */

  /* USER CODE END LPUART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();

    /**LPUART1 GPIO Configuration
    PA2     ------> LPUART1_TX
    PA3     ------> LPUART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

  /* USER CODE BEGIN LPUART1_MspDeInit 1 */

  /* USER CODE END LPUART1_MspDeInit 1 */
  }

}

///**
//* @brief RTC MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hrtc: RTC handle pointer
//* @retval None
//*/
//void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
//{
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//  if(hrtc->Instance==RTC)
//  {
//  /* USER CODE BEGIN RTC_MspInit 0 */
//
//  /* USER CODE END RTC_MspInit 0 */
//
//  /** Initializes the peripherals clocks
//  */
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
//    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Peripheral clock enable */
//    __HAL_RCC_RTC_ENABLE();
//    __HAL_RCC_RTCAPB_CLK_ENABLE();
//  /* USER CODE BEGIN RTC_MspInit 1 */
//
//  /* USER CODE END RTC_MspInit 1 */
//  }
//
//}
//
///**
//* @brief RTC MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hrtc: RTC handle pointer
//* @retval None
//*/
//void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
//{
//  if(hrtc->Instance==RTC)
//  {
//  /* USER CODE BEGIN RTC_MspDeInit 0 */
//
//  /* USER CODE END RTC_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_RTC_DISABLE();
//    __HAL_RCC_RTCAPB_CLK_DISABLE();
//  /* USER CODE BEGIN RTC_MspDeInit 1 */
//
//  /* USER CODE END RTC_MspDeInit 1 */
//  }
//
//}
//
///**
//* @brief SPI MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hspi: SPI handle pointer
//* @retval None
//*/
//void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hspi->Instance==SPI2)
//  {
//  /* USER CODE BEGIN SPI2_MspInit 0 */
//
//  /* USER CODE END SPI2_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_SPI2_CLK_ENABLE();
//
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**SPI2 GPIO Configuration
//    PB12     ------> SPI2_NSS
//    PB13     ------> SPI2_SCK
//    PB14     ------> SPI2_MISO
//    PB15     ------> SPI2_MOSI
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//  /* USER CODE BEGIN SPI2_MspInit 1 */
//
//  /* USER CODE END SPI2_MspInit 1 */
//  }
//
//}
//
///**
//* @brief SPI MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hspi: SPI handle pointer
//* @retval None
//*/
//void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
//{
//  if(hspi->Instance==SPI2)
//  {
//  /* USER CODE BEGIN SPI2_MspDeInit 0 */
//
//  /* USER CODE END SPI2_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_SPI2_CLK_DISABLE();
//
//    /**SPI2 GPIO Configuration
//    PB12     ------> SPI2_NSS
//    PB13     ------> SPI2_SCK
//    PB14     ------> SPI2_MISO
//    PB15     ------> SPI2_MOSI
//    */
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
//
//  /* USER CODE BEGIN SPI2_MspDeInit 1 */
//
//  /* USER CODE END SPI2_MspDeInit 1 */
//  }
//
//}
//
///**
//* @brief PCD MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hpcd: PCD handle pointer
//* @retval None
//*/
//void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
//{
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//  if(hpcd->Instance==USB)
//  {
//  /* USER CODE BEGIN USB_MspInit 0 */
//
//  /* USER CODE END USB_MspInit 0 */
//
//  /** Initializes the peripherals clocks
//  */
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
//    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Peripheral clock enable */
//    __HAL_RCC_USB_CLK_ENABLE();
//  /* USER CODE BEGIN USB_MspInit 1 */
//
//  /* USER CODE END USB_MspInit 1 */
//  }
//
//}
//
///**
//* @brief PCD MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hpcd: PCD handle pointer
//* @retval None
//*/
//void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
//{
//  if(hpcd->Instance==USB)
//  {
//  /* USER CODE BEGIN USB_MspDeInit 0 */
//
//  /* USER CODE END USB_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_USB_CLK_DISABLE();
//  /* USER CODE BEGIN USB_MspDeInit 1 */
//
//  /* USER CODE END USB_MspDeInit 1 */
//  }
//
//}
//
//extern DMA_HandleTypeDef hdma_sai1_a;
//
//extern DMA_HandleTypeDef hdma_sai1_b;
//
//static uint32_t SAI1_client =0;
//
//void HAL_SAI_MspInit(SAI_HandleTypeDef* hsai)
//{
//
//  GPIO_InitTypeDef GPIO_InitStruct;
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
///* SAI1 */
//    if(hsai->Instance==SAI1_Block_A)
//    {
//    /* Peripheral clock enable */
//
//  /** Initializes the peripherals clocks
//  */
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
//    PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_SYSCLK;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    if (SAI1_client == 0)
//    {
//       __HAL_RCC_SAI1_CLK_ENABLE();
//    }
//    SAI1_client ++;
//
//    /**SAI1_A_Block_A GPIO Configuration
//    PA8     ------> SAI1_SCK_A
//    PA9     ------> SAI1_FS_A
//    PA10     ------> SAI1_SD_A
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_8|SAI1_FS_A_IO_SX_LO_RDY_Pin|GPIO_PIN_10;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF14_SAI1;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//      /* Peripheral DMA init*/
//
//    hdma_sai1_a.Instance = DMA1_Channel1;
//    hdma_sai1_a.Init.Request = DMA_REQUEST_SAI1_A;
//    hdma_sai1_a.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    hdma_sai1_a.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_sai1_a.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_sai1_a.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_sai1_a.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_sai1_a.Init.Mode = DMA_CIRCULAR;
//    hdma_sai1_a.Init.Priority = DMA_PRIORITY_LOW;
//    if (HAL_DMA_Init(&hdma_sai1_a) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Several peripheral DMA handle pointers point to the same DMA handle.
//     Be aware that there is only one channel to perform all the requested DMAs. */
//    __HAL_LINKDMA(hsai,hdmarx,hdma_sai1_a);
//
//    __HAL_LINKDMA(hsai,hdmatx,hdma_sai1_a);
//
//    }
//    if(hsai->Instance==SAI1_Block_B)
//    {
//      /* Peripheral clock enable */
//
//  /** Initializes the peripherals clocks
//  */
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
//    PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_SYSCLK;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//      if (SAI1_client == 0)
//      {
//       __HAL_RCC_SAI1_CLK_ENABLE();
//      }
//    SAI1_client ++;
//
//    /**SAI1_B_Block_B GPIO Configuration
//    PB5     ------> SAI1_SD_B
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_5;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF12_SAI1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//      /* Peripheral DMA init*/
//
//    hdma_sai1_b.Instance = DMA1_Channel2;
//    hdma_sai1_b.Init.Request = DMA_REQUEST_SAI1_B;
//    hdma_sai1_b.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_sai1_b.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_sai1_b.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_sai1_b.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_sai1_b.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_sai1_b.Init.Mode = DMA_CIRCULAR;
//    hdma_sai1_b.Init.Priority = DMA_PRIORITY_LOW;
//    if (HAL_DMA_Init(&hdma_sai1_b) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Several peripheral DMA handle pointers point to the same DMA handle.
//     Be aware that there is only one channel to perform all the requested DMAs. */
//    __HAL_LINKDMA(hsai,hdmarx,hdma_sai1_b);
//    __HAL_LINKDMA(hsai,hdmatx,hdma_sai1_b);
//    }
//}
//
//void HAL_SAI_MspDeInit(SAI_HandleTypeDef* hsai)
//{
///* SAI1 */
//    if(hsai->Instance==SAI1_Block_A)
//    {
//    SAI1_client --;
//    if (SAI1_client == 0)
//      {
//      /* Peripheral clock disable */
//       __HAL_RCC_SAI1_CLK_DISABLE();
//      }
//
//    /**SAI1_A_Block_A GPIO Configuration
//    PA8     ------> SAI1_SCK_A
//    PA9     ------> SAI1_FS_A
//    PA10     ------> SAI1_SD_A
//    */
//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8|SAI1_FS_A_IO_SX_LO_RDY_Pin|GPIO_PIN_10);
//
//    /* SAI1 DMA Deinit */
//    HAL_DMA_DeInit(hsai->hdmarx);
//    HAL_DMA_DeInit(hsai->hdmatx);
//    }
//    if(hsai->Instance==SAI1_Block_B)
//    {
//    SAI1_client --;
//      if (SAI1_client == 0)
//      {
//      /* Peripheral clock disable */
//      __HAL_RCC_SAI1_CLK_DISABLE();
//      }
//
//    /**SAI1_B_Block_B GPIO Configuration
//    PB5     ------> SAI1_SD_B
//    */
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5);
//
//    /* SAI1 DMA Deinit */
//    HAL_DMA_DeInit(hsai->hdmarx);
//    HAL_DMA_DeInit(hsai->hdmatx);
//    }
//}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
