/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_previpass.h
 * @author  Kinéis
 * @brief subset of AT commands concerning satellite PASS predictions, usefull for Medium Acces
 *
 * @note This subset of commands are actually stubs, only present because GUI is sending such AT
 *       commands to KIM device.
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_PREVIPASS_H
#define __MGR_SPI_CMD_PREVIPASS_H

/* Includes ------------------------------------------------------------------*/
#include "mgr_at_cmd_common.h"


/* Functions -----------------------------------------------------------------*/

/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_READPREPASSEN_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_WRITEPREPASSENREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_WRITEPREPASSEN_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief Process AT command "AT+UDATE" get/set the current user date and UTC time.
 *
 */
bool bMGR_SPI_CMD_READUDATE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief Process AT command "AT+UDATE" get/set the current user date and UTC time.
 *
 */
bool bMGR_SPI_CMD_WRITEUDATEREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
/** @brief Process AT command "AT+UDATE" get/set the current user date and UTC time.
 *
 */
bool bMGR_SPI_CMD_WRITEUDATE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
#endif /* __MGR_SPI_CMD_PREVIPASS_H */

/**
 * @}
 */
