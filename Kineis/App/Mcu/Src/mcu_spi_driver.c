// SPDX-License-Identifier: no SPDX license
/**
 * @file    mcu_at_spi.c
 * @author  Arribada
 * @brief   MCU wrapper for AT SPI
 */

/**
 * @addtogroup MCU_APP_WRAPPERS
 * @brief MCU wrapper used by Kineis Application example.
 *
 * One has to implement API as per its microcontroller and its platform ressources.
 * This version is for STM32 uC such as STM32WLE5xx, STM32WL55xx.
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "kineis_sw_conf.h" // for assert include below
#include KINEIS_SW_ASSERT_H
#include "mgr_log.h"
#include "mcu_spi_driver.h"

/* Defines -------------------------------------------------------------------*/


/* Variables -----------------------------------------------------------------*/
static SPI_HandleTypeDef *hspi_handle = NULL;

static uint8_t spiTxBuf[TXBUF_SIZE];
static uint8_t spiRxBuf[RXBUF_SIZE];
SPI_Buffer rxBuf = { .data = spiRxBuf, .size = RXBUF_SIZE, .next_req = 1};
SPI_Buffer txBuf = { .data = spiTxBuf, .size = TXBUF_SIZE, .next_req = 0};


//static bool (*rxEvtCb)(uint8_t *pu8_RxBuffer, int16_t *pi16_nbRxValidChar);
static int8_t (*rxSpiEvtCb)(SPI_Buffer *rx, SPI_Buffer *tx);

/* Private function prototypes -----------------------------------------------*/


// /* Functions -----------------------------------------------------------------*/
// Callback when a command is received
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI1) {
        MGR_LOG_DEBUG("RX completed\r\n");
        //uint8_t command = spiRxBuf[0];
        if (rxSpiEvtCb != NULL)
        {
        	//rxBuf.size = 1;
        	rxSpiEvtCb(&rxBuf, &txBuf);
        } else {
			MGR_LOG_DEBUG("%s:: rxSpiEvtCb not defined\r\n", __func__);
        }


    }
}

// Callback for SPI TxRx completion (optional for debugging)
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI1) {
        MGR_LOG_DEBUG("TX-RX completed\r\n");
    }
	txBuf.next_req = 0;
	rxBuf.next_req = 1;
	HAL_SPI_Receive_IT(hspi, rxBuf.data, rxBuf.next_req);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI1) {
        MGR_LOG_DEBUG("TX completed\n\r");

    }
}

HAL_StatusTypeDef MCU_SPI_DRIVER_writeread()
{
	HAL_StatusTypeDef ret = HAL_OK;
	// waiting Read request
	ret = HAL_SPI_TransmitReceive_IT(hspi_handle, txBuf.data, rxBuf.data, txBuf.next_req);
	return ret;
}

HAL_StatusTypeDef MCU_SPI_DRIVER_read()
{
	if(rxBuf.next_req < 1) {
		MGR_LOG_DEBUG("%s:: Waiting RX is %u should be greater than 0\n\r",__func__, rxBuf.next_req);
		rxBuf.next_req = 1; // next request forced to 1
	}
	return(HAL_SPI_Receive_IT(hspi_handle, rxBuf.data, rxBuf.next_req));
}



bool MCU_SPI_DRIVER_register(void *handle, int8_t (*rx_spi_evt_cb)(SPI_Buffer *rx, SPI_Buffer *tx))
{
	HAL_StatusTypeDef ret = HAL_OK;
	MGR_LOG_DEBUG("%s:: called\n\r", __func__);
	if (handle != NULL && hspi_handle == NULL) // Handle is not set yet
	{
		hspi_handle = (SPI_HandleTypeDef *)handle;
    } else if (handle == NULL && hspi_handle == NULL) // Handle is already set
	{
		return false; 
	}
	// Set SPI OK and TX WAITING flags
	txBuf.next_req = 0;
	rxBuf.next_req = 1;
	// Start receiving the command
	ret = HAL_SPI_Receive_IT(hspi_handle, rxBuf.data, rxBuf.next_req);
    if (ret == HAL_OK) // Want to read the command received
    {
    	if (rx_spi_evt_cb != NULL)
    	{
			rxSpiEvtCb = rx_spi_evt_cb;
    	} else {
			return false; 
		}
    	return true;
    } else {
    	return false;
    }
}



/**
  * @brief  UART error callback. Can raise in case of UART OVERFLOW, DMA RX ERROR, ...
 *
 * This function is highly based on STM32HAL_UART. It overrides the generic defined error callback
 *
 * @note So far, this callback is emptied
 *
  * @param  huart UART handle.
  * @retval None
  */
void HAL_SPI_ErrorCallback(__attribute__((unused)) SPI_HandleTypeDef *hspi)
{

	// Set SPI OK and TX WAITING flags
	MGR_LOG_DEBUG("%s:: called\n\r", __func__);
    // Check for Overrun Error
    if (hspi->ErrorCode & HAL_SPI_ERROR_OVR) {
        // Clear the Overrun flag
		MGR_LOG_DEBUG("%s:: Clear Overrun flag\n\r", __func__);
        __HAL_SPI_CLEAR_OVRFLAG(hspi);
    }

    // Check for Mode Fault Error
    if (hspi->ErrorCode & HAL_SPI_ERROR_MODF) {
        // Clear the Mode Fault flag
		MGR_LOG_DEBUG("%s:: Clear Mode Fault flag\n\r", __func__);
        __HAL_SPI_CLEAR_MODFFLAG(hspi);
    }

    // Check for Frame Error
    if (hspi->ErrorCode & HAL_SPI_ERROR_FRE) {
        // Clear the Frame Error flag
		MGR_LOG_DEBUG("%s:: Clear Frame Error flag\n\r", __func__);
        __HAL_SPI_CLEAR_FREFLAG(hspi);
    }

    // Reset the ErrorCode
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    // Reset the SPI state
    hspi->State = HAL_SPI_STATE_READY;

    // Optionally reinitialize SPI
    HAL_SPI_DeInit(hspi);
    HAL_SPI_Init(hspi);
    MCU_SPI_DRIVER_register(hspi, NULL);

}

/**
 * @}
 */
