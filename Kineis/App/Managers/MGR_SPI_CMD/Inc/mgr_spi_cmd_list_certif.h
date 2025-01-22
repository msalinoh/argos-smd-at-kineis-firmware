/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_certif.h
 * @author  Kinéis
 * @brief subset of AT commands concerning specifics for certification
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_CERTIF_H
#define __MGR_SPI_CMD_CERTIF_H

/* Includes ------------------------------------------------------------------*/

/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_READCW_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_WRITECWREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);
/** @brief

 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_SPI_CMD_WRITECW_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

#endif /* __MGR_SPI_CMD_CERTIF_H */

/**
 * @}
 */
