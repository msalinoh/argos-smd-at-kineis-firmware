// SPDX-License-Identifier: no SPDX license
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
static char au8_output[LOGGING_PURPOSE_STORAGE_MAX_MESSAGE_LEN];

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/
void vMGR_LOG_printf(const char *format, ...)
{
	va_list args;

	/* Reset output buffer */
	memset(au8_output, 0x00, LOGGING_PURPOSE_STORAGE_MAX_MESSAGE_LEN);

	/* Log message formatting */
	va_start(args, format);
	vsprintf(au8_output, format, args);
	va_end(args);
	assert_param(strlen(au8_output) < sizeof(au8_output));

	/* Send log message via UART */
	HAL_UART_Transmit(&log_uart, (uint8_t *)au8_output, strlen(au8_output), 500);
}
#endif // end USE_LOCAL_PRINTF

/**
 * @}
 */
