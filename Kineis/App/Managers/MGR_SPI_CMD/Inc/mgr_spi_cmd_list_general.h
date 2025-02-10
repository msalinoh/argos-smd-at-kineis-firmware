/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_general.h
 * @author  Kineis
 * @brief subset of AT commands concerning general purpose (get ID, FW version, ...)
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_LIST_GENERAL_H
#define __MGR_SPI_CMD_LIST_GENERAL_H

/* Includes ------------------------------------------------------------------*/
#include "mgr_at_cmd_common.h"
#include "mcu_spi_driver.h"
/* Define value --------------------------------------------------------------*/


/* Extern variable -----------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/


/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_VERSION_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);


/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_PING_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_MACSTATUS_cmd(SPI_Buffer *rx, SPI_Buffer *tx);


/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_SPISTATE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READSPIVERSION_cmd(SPI_Buffer *rx, SPI_Buffer *tx);


/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READFIRMWARE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);


/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READADDRESS_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITEADDRESSREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITEADDRESS_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READID_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITEIDREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITEID_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READSN_cmd(SPI_Buffer *rx, SPI_Buffer *tx);


/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READRCONF_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITERCONFREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITERCONF_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_SAVERCONF_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READLPM_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITELPMREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITELPM_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READSECKEY_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITESECKEYREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITESECKEY_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
/** @brief
 *
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_READSPIMACSTATE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
#endif /* __MGR_SPI_CMD_LIST_GENERAL_H */
