/**
 * @file    mgr_log.c
 * @brief   manager the output log using the LPUART communication driver
 * @author  Ahmed JEDIDI
 * @date    creation 03/02/2020
 * @version 1.0
 * @note
 */

/**
 * @addtogroup MGR_LOG
 * @{
 */

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "mgr_log.h"


#ifdef USE_LOCAL_PRINTF

/*******************************************************************************
 * DEFINES
 ******************************************************************************/
#define LOGGING_PURPOSE_STORAGE_MAX_MESSAGE_LEN 256

/*******************************************************************************
 * VARIABLES
 ******************************************************************************/
#ifndef UNIT_TEST
static
#endif
char au8_output[LOGGING_PURPOSE_STORAGE_MAX_MESSAGE_LEN];

#ifdef FLASHLOG_ENABLED
/* \note cannot use __section here because of conflict in Cmock, use __attribute instead
 * __attribute__((__section__(".data_sram2")))
 * __section(".data_sram2")
 */
__attribute__((__section__(".data_sram2")))
bool is_flashlogging_enabled = false;
#endif // end FLASHLOG_ENABLED

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/
#ifdef FLASHLOG_ENABLED
bool bMGR_LOG_isFlashLoggingEnabled(void)
{
	return is_flashlogging_enabled;
}

void vMGR_LOG_enableFlashLogging(bool b_isEnable)
{
	is_flashlogging_enabled = b_isEnable;
}
#endif //FLASHLOG_ENABLED

void vMGR_LOG_printf(const char *format, ...)
{
	va_list args;

	/* Reset output buffer */
	memset(au8_output, 0x00, LOGGING_PURPOSE_STORAGE_MAX_MESSAGE_LEN);

	/* Log message formatting */
	va_start(args, format);
	vsprintf(au8_output, format, args);
	va_end(args);

	/* Send log message via UART */
	HAL_UART_Transmit(&log_uart, (uint8_t *)au8_output, strlen(au8_output), 500);
}
#endif // end USE_LOCAL_PRINTF

/**
 * @brief Write content of data buffer as AT cmd response
 *
 * @param[in] pu8_inDataBuff: pointer to data buffer
 * @param[in] u16_dataLenBit: length of data buffer in bit
 *
 * @return none
 */
void vMGR_LOG_AT_dataBuf(uint8_t *pu8_inDataBuff, uint16_t u16_dataLenBit)
{
	uint16_t u16_remainingBits;
	const uint16_t u16_dataLenByte_trunc = u16_dataLenBit >> 3;
	uint8_t *pu8_hex;

	/*
	 * Convert data hex to ascii per byte then last byte is
	 * treated per nibble
	 */

	for (pu8_hex = pu8_inDataBuff;
			(pu8_hex < (pu8_inDataBuff + u16_dataLenByte_trunc));
			pu8_hex++)
		MGR_LOG_AT("%02X", *pu8_hex);

	u16_remainingBits = u16_dataLenBit - (u16_dataLenByte_trunc << 3);
	if (u16_remainingBits > 4) {
		MGR_LOG_AT("%02X", *pu8_hex);
	} else if (u16_remainingBits > 0) {
		MGR_LOG_AT("%01X", *pu8_hex);
	} /* else no additional bits */
}

/**
 * @}
 */
