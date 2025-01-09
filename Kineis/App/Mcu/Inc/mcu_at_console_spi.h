/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    mcu_at_spi.h
 * @author  Arribada
 * @brief   MCU wrapper for AT SPI control
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_AT_CONSOLE_SPI_H
#define __MCU_AT_CONSOLE_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "stm32wlxx_hal.h"
#include "stm32wlxx_hal_spi.h"

/* Functions prototypes ------------------------------------------------------*/


bool MCU_AT_SPI_register(void *context,
		bool (*rx_evt_cb)(uint8_t *pu8_RxBuffer, int16_t *pi16_nbRxValidChar));


void MCU_AT_SPI_send(const char *format, ...);

void MCU_AT_SPI_send_dataBuf(uint8_t *pu8_inDataBuff, uint16_t u16_dataLenBit);

#endif /* __MCU_AT_CONSOLE_SPI_H */

/**
 * @}
 */
