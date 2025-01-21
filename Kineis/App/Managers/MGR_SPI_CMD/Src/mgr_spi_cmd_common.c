// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_at_cmd_common.c
 * @author Kineis
 * @brief common part of the AT cmd manager (logging, AT cmd response api)
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "mgr_spi_cmd_common.h"
#include "user_data.h"
#include "kns_mac.h"
#include "mgr_log.h"

/* Public functions ----------------------------------------------------------*/

bool bMGR_SPI_CMD_logFailedMsg(enum ERROR_RETURN_T eErrorType, SPI_Buffer *tx)
{
	MGR_LOG_DEBUG("+ERROR=%i\r\n", eErrorType);
	tx->data[0] = 0;
	tx->next_req = 1;
	MCU_SPI_DRIVER_resp();
	return false;
}
uint8_t bMGR_SPI_DRIVER_wait_next()
{
	uint8_t ret = 0;
	ret = MCU_SPI_DRIVER_wait_next();
	spiState = SPICMD_IDLE;
	return ret;

}
uint8_t bMGR_SPI_DRIVER_resp()
{
	uint8_t ret = 0;
	ret = MCU_SPI_DRIVER_resp();
	spiState = SPICMD_IDLE;
	return ret;

}


/**
 * @}
 */
