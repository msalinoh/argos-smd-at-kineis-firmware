// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_spi_cmd_list_certif.c
 * @author Arribada
 * @brief subset of SPI commands concerning satellite PASS predictions, usefull for Medium Acces
 *
 * @note This subset of commands are actually stubs, not yet implemented
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "mcu_spi_driver.h"
#include "mgr_spi_cmd_list_certif.h"
#include "mgr_spi_cmd_common.h"
#include "mgr_log.h"

/* Functions -----------------------------------------------------------------*/

/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_READCW_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
{
	MGR_LOG_VERBOSE("[ERROR] Status mode is unauthorized for this AT cmd\r\n");
	uint8_t ret = HAL_OK;
	tx->data[0] = 0;
	tx->next_req = 1;
	rx->next_req = 1;
	ret = bMGR_SPI_DRIVER_writeread();

	if (ret == HAL_OK)
	{
		return true;
	} else {
		return false;
	}
}

/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_WRITECWREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
{
	MGR_LOG_VERBOSE("[ERROR] Status mode is unauthorized for this AT cmd\r\n");
	uint8_t ret = HAL_OK;
	tx->data[0] = 0;
	tx->next_req = 1;
	rx->next_req = 1;
	ret = bMGR_SPI_DRIVER_writeread();

	if (ret == HAL_OK)
	{
		return true;
	} else {
		return false;
	}
}

/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_WRITECW_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
{
	MGR_LOG_VERBOSE("[ERROR] Status mode is unauthorized for this AT cmd\r\n");
	uint8_t ret = HAL_OK;
	tx->data[0] = 0;
	tx->next_req = 1;
	rx->next_req = 1;
	ret = bMGR_SPI_DRIVER_writeread();

	if (ret == HAL_OK)
	{
		return true;
	} else {
		return false;
	}
}

/**
 * @}
 */
