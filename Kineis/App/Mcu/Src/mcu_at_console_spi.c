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
#include "kns_app_conf.h" // for STM32 HAL include on UART
#include STM32_HAL_H
#include "kineis_sw_conf.h" // for assert include below
#include KINEIS_SW_ASSERT_H
#include "mgr_log.h"

/* Defines -------------------------------------------------------------------*/
#if defined(STM32WLE5xx) || defined(STM32WL55xx)
#define USART_ISR_RXNE USART_ISR_RXNE_RXFNE
#endif

#ifdef USE_HDA4
#define TXBUF_SIZE 2560
#define RXBUF_SIZE 2560
#else
#define TXBUF_SIZE 256
#define RXBUF_SIZE 256
#endif

/* Variables -----------------------------------------------------------------*/

static SPI_HandleTypeDef *hspi_handle;
static char spiTxBuf[TXBUF_SIZE];
static uint8_t spiRxBuf[RXBUF_SIZE];

static bool (*rxEvtCb)(uint8_t *pu8_RxBuffer, int16_t *pi16_nbRxValidChar);

/* Private function prototypes -----------------------------------------------*/

/** @brief RX interrupt handler for 7 or 8 bits data word length .
 *
 * This function is highly based on STM32HAL_UART expect callback is called at each received
 * character. This allow to treat RX received characters as a continuous stream.
 *
 * At each received character, the callback is said to consume data, it updates the buffer pointer
 * and the number-of-valid-char reduced by what was consummed.
 *
 * @note In case of overflow on RX buffer, force write from the beginning of the buffer pRxBuffPtr.
 *       thus, the entire buffer may be lost. Actually, such case may only happen in case:
 *       * user sent too much data in console, this is not an expected scenario
 *       * user has sent some garbage before its several AT cmds, leading accumuation to overflow.
 *       This is not an expected behaviour as well.
 *
 * @todo circular buffer may be implemented instead of overflowing sometimes.
 *
 * STM32HAL_UART base function prototype:
 @verbatim
static void UART_RxISR_8BIT(UART_HandleTypeDef *huart)
 @endverbatim
 *
 * @param huart UART handle.
 * @retval None
 */
static void SPI_RxISR_8BIT_CUSTOM(SPI_HandleTypeDef *hspi)
{
	uint16_t uhdata;
	uint8_t *pu8_RxBuffer = NULL;
	int16_t i16_nbRxValidChar = 0;
	const uint8_t uhMask = 0xFF;

	i16_nbRxValidChar = hspi->RxXferSize - hspi->RxXferCount;
	pu8_RxBuffer = hspi->pRxBuffPtr - i16_nbRxValidChar;
	//< pu8_RxBuffer points to the 1st element of the RX array

	/* Check that a Rx process is ongoing */
	if (hspi->State == HAL_SPI_STATE_BUSY_RX) {
		/* Consume all incoming data in the same interrupt. RXNE is set by HW and cleared by reading RDR reg */
		while ((READ_REG(hspi->Instance->SR) & SPI_SR_RXNE) != 0U) {
			uhdata = (uint16_t) READ_REG(hspi->Instance->DR);
		    //*hspi->pRxBuffPtr = (*(__IO uint8_t *)hspi->Instance->DR);
			*hspi->pRxBuffPtr = (uint8_t)(uhdata & (uint8_t)uhMask);

			/* Circular buffer handling: Reset pointer if buffer is full */
			if (hspi->RxXferCount == 1U) {
				hspi->pRxBuffPtr = hspi->pRxBuffPtr - hspi->RxXferSize + hspi->RxXferCount;
				hspi->RxXferCount = hspi->RxXferSize;
			} else {
				hspi->pRxBuffPtr++;
				hspi->RxXferCount--;
			}
		}

		/* Call the reception event callback if defined */
		if (rxEvtCb != NULL) {
			i16_nbRxValidChar = hspi->RxXferSize - hspi->RxXferCount;
			pu8_RxBuffer = hspi->pRxBuffPtr - i16_nbRxValidChar;
			//< pu8_RxBuffer points to the 1st element of the RX array

//		    // Display received characters
//		    MGR_LOG_DEBUG_RAW("Received chars: ");
//		    for (int i = 0; i < i16_nbRxValidChar; i++) {
//		        char receivedChar = (char)pu8_RxBuffer[i];
//		        if (isprint(receivedChar)) { // Check if the character is printable
//		            MGR_LOG_DEBUG_RAW("%c", receivedChar);
//		        } else {
//		            MGR_LOG_DEBUG_RAW("\\x%02X", pu8_RxBuffer[i]); // Display non-printable as hex
//		        }
//		    }
//		    MGR_LOG_DEBUG_RAW("\n");

			while (rxEvtCb(pu8_RxBuffer, &i16_nbRxValidChar) == true) {
				// empty loop: callback processes data
			}

			//< Adjust the pointer to align with the next start of data
			hspi->pRxBuffPtr = pu8_RxBuffer + i16_nbRxValidChar;
			hspi->RxXferCount = hspi->RxXferSize - i16_nbRxValidChar;
		}
	} else {
		/* Clear RXNE interrupt flag if no ongoing reception */
		uint16_t dummy_read = (uint16_t) READ_REG(hspi->Instance->DR);
		UNUSED(dummy_read);
	}
}

/** @brief Enable and start RX interrupt from UART
 *
 * This function is highly based on STM32HAL_UART expect it called internal interrupt handler
 * instead of generic one.
 *
  @verbatim
HAL_StatusTypeDef UART_Start_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
  @endverbatim
 * @param huart UART handle.
 * @param pData Pointer to data buffer (u8 or u16 data elements).
 * @param Size  Amount of data elements (u8 or u16) to be received.
 * @retval HAL status
 */
static HAL_StatusTypeDef SPI_StartRx_IT(SPI_HandleTypeDef *hspi, uint8_t *pData,
						uint16_t Size)
{
    if (hspi == NULL)
        return HAL_ERROR;

	// Handle received data

    /* Check that a Rx process is not already ongoing */
    if (hspi->State == HAL_SPI_STATE_READY) {
        if ((pData == NULL) || (Size == 0U))
            return HAL_ERROR;

        __HAL_LOCK(hspi);

        hspi->pRxBuffPtr = pData;
        hspi->RxXferSize = Size;
        hspi->RxXferCount = Size;
        //hspi->RxISR = SPI_RxISR_8BIT;

        /* Init field not used in handle to zero */
        hspi->pTxBuffPtr  = (uint8_t *)NULL;
        hspi->TxXferSize  = 0U;
        hspi->TxXferCount = 0U;
        hspi->TxISR       = NULL;

        hspi->ErrorCode = HAL_SPI_ERROR_NONE;
        hspi->State = HAL_SPI_STATE_BUSY_RX;

        /* Set RX Fifo threshold according the reception data length: 8 bit */
        SET_BIT(hspi->Instance->CR2, SPI_RXFIFO_THRESHOLD);
        hspi->RxISR = SPI_RxISR_8BIT_CUSTOM;

        /* Enable TXE and ERR interrupt */
         __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));

         /* Note : The SPI must be enabled after unlocking current process
                   to avoid the risk of SPI interrupt handle execution before current
                   process unlock */

         /* Check if the SPI is already enabled */
         if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
         {
           /* Enable SPI peripheral */
           __HAL_SPI_ENABLE(hspi);
         }

        /* Enable SPI Error Interrupt (Overrun error) */
        SET_BIT(hspi->Instance->CR2, SPI_CR2_ERRIE);

        /* Enable RXNE interrupt */
        SET_BIT(hspi->Instance->CR2, SPI_CR2_RXNEIE);

        __HAL_UNLOCK(hspi);

        return HAL_OK;
    } else {
        return HAL_BUSY;
    }
}

/* Functions -----------------------------------------------------------------*/

bool MCU_AT_CONSOLE_register(void *handle, bool (*rx_evt_cb)(uint8_t *pu8_RxBuffer,
	int16_t *pi16_nbRxValidChar))
{
	hspi_handle = (SPI_HandleTypeDef *)handle;


	if (SPI_StartRx_IT(hspi_handle, spiRxBuf, sizeof(spiRxBuf)) == HAL_OK) {
		if (rx_evt_cb != NULL)
		{
			rxEvtCb = rx_evt_cb;
		}
		return true;
	} else
		return false;
}

void MCU_AT_CONSOLE_send(const char *format, ...)
{
	va_list args;

	/* Reset output buffer */
	memset(spiTxBuf, 0x00, sizeof(spiTxBuf));

	va_start(args, format);
	vsprintf(spiTxBuf, format, args);
	va_end(args);

	/* Ensure the buffer is not overflowed */
	kns_assert(strlen(spiTxBuf) < sizeof(spiTxBuf));

	/* Send log message via SPI */
	if (hspi_handle != NULL)
	{
        hspi_handle->State = HAL_SPI_STATE_READY;
		if (HAL_SPI_Transmit(hspi_handle, (uint8_t *)spiTxBuf, strlen(spiTxBuf), 500) != HAL_OK)
		{
			/* Transmission error handling */
			//kns_assert(0);
		}
		MCU_AT_CONSOLE_register(hspi_handle, NULL);
	}
	else
	{
		/* SPI handle must be initialized before use */
		kns_assert(0);
	}
}

void MCU_AT_CONSOLE_send_dataBuf(uint8_t *pu8_inDataBuff, uint16_t u16_dataLenBit)
{
	uint16_t u16_remainingBits;
	const uint16_t u16_dataLenByte_trunc = u16_dataLenBit >> 3;
	uint8_t *pu8_hex;

	/*
	 * Convert data hex to ascii per byte then last byte is
	 * treated per nibble
	 */

//	for (pu8_hex = pu8_inDataBuff;
//			(pu8_hex < (pu8_inDataBuff + u16_dataLenByte_trunc));
//			pu8_hex++)
//		MCU_AT_SPI_send("%c", *pu8_hex);

	for (pu8_hex = pu8_inDataBuff;
			(pu8_hex < (pu8_inDataBuff + u16_dataLenByte_trunc));
			pu8_hex++)
		MCU_AT_CONSOLE_send("%02X", *pu8_hex);

	u16_remainingBits = u16_dataLenBit - (u16_dataLenByte_trunc << 3);
	if (u16_remainingBits > 4)
		MCU_AT_CONSOLE_send("%02X", *pu8_hex);
	else if (u16_remainingBits > 0)
		MCU_AT_CONSOLE_send("%01X", *pu8_hex);
	/* else no additional bits */
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
	//MGR_LOG("\r\n+SPI_ERROR (OVR, RX error) => PLEASE RESET MODULE.\r\n\r\n");
    MGR_LOG_DEBUG("SPI_ERROR (OVR, RX error)\r\n");
    MCU_AT_CONSOLE_register(hspi, NULL);
	//kns_assert(0);
}

/**
 * @}
 */
