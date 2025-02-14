/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_spi_cmd_list_mac.h
 * @brief Subset of SPI commands concerning Kinéis Medium Access Channel (MAC).
 *
 * This file declares the SPI commands used for interacting with the Kinéis MAC.
 * These functions are implemented as stubs and exist solely to handle commands sent by the GUI to the KIM device.
 *
 * @note Although these commands are stubs, they are required for compatibility with the GUI interface.
 *
 * @author Arribada
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */

#ifndef __MGR_SPI_CMD_MAC_H
#define __MGR_SPI_CMD_MAC_H

/* Includes ------------------------------------------------------------------*/
#include "mgr_spi_cmd_common.h"

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Read the Kinéis MAC configuration.
 *
 * This function processes the SPI command to read the current MAC configuration.
 * Although implemented as a stub, it is designed to handle requests from the GUI.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the command.
 * @param[out] tx Pointer to the SPI transmit buffer where the MAC configuration data will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_READKMAC_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Initiate a request to write the Kinéis MAC configuration.
 *
 * This function processes the SPI command that initiates the procedure for updating the MAC configuration.
 * As a stub, this function acknowledges the write request from the GUI.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the request.
 * @param[out] tx Pointer to the SPI transmit buffer where the acknowledgment will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_WRITEKMACREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Write the new Kinéis MAC configuration.
 *
 * This function processes the SPI command to update the MAC configuration after a valid write request.
 * Although implemented as a stub, it processes the data provided by the GUI.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the new MAC configuration data.
 * @param[out] tx Pointer to the SPI transmit buffer where the result of the write operation will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_WRITEKMAC_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

#endif /* __MGR_SPI_CMD_MAC_H */

/**
 * @}
 */
