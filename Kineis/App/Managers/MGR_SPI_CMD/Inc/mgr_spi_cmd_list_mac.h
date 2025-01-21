/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_mac.h
 * @author Kinéis
 * @brief subset of AT commands concerning Kinéis Medium Acces Channel (MAC).
 *
 * @note This subset of commands are actually stubs, only present because GUI is sending such AT
 *       commands to KIM device.
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */


#ifndef __MGR_SPI_CMD_MAC_H
#define __MGR_SPI_CMD_MAC_H

/* Includes ------------------------------------------------------------------*/

#include "mgr_spi_cmd_common.h"

/* Functions -----------------------------------------------------------------*/

/**
 * @brief
 */
bool bMGR_SPI_CMD_READKMAC_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief
 */
bool bMGR_SPI_CMD_WRITEKMACREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
/**
 * @brief
 */
bool bMGR_SPI_CMD_WRITEKMAC_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
#endif /* __MGR_SPI_CMD_MAC_H */
/**
 * @}
 */
