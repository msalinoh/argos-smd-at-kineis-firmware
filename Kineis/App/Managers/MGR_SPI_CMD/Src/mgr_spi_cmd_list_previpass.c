// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_spi_cmd_list_previpass.c
 * @author Kinéis
 * @brief subset of AT commands concerning satellite PASS predictions, usefull for Medium Acces
 *
 * @note This subset of commands are actually stubs, only present because GUI is sending such AT
 *       commands to KIM device.
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "mcu_spi_driver.h"
#include "mgr_spi_cmd_common.h"
#include "mgr_spi_cmd_list_previpass.h"
#include "mgr_log.h"

/* Functions -----------------------------------------------------------------*/

/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_READPREPASSEN_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
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
bool bMGR_SPI_CMD_WRITEPREPASSENREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
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
bool bMGR_SPI_CMD_WRITEPREPASSEN_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
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


/** @brief Process AT command "AT+UDATE" get/set the current user date and UTC time.
 *
 */
bool bMGR_SPI_CMD_READUDATE_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
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


/** @brief Process AT command "AT+UDATE" get/set the current user date and UTC time.
 *
 */
bool bMGR_SPI_CMD_WRITEUDATEREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
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

/** @brief Process AT command "AT+UDATE" get/set the current user date and UTC time.
 *
 */
bool bMGR_SPI_CMD_WRITEUDATE_cmd(SPI_Buffer *rx, SPI_Buffer *tx)
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
