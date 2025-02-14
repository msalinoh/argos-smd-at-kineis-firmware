/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_spi_cmd_list_previpass.h
 * @brief Subset of SPI commands concerning satellite PASS predictions, useful for Medium Access.
 *
 * This file declares stub functions for handling SPI commands related to satellite PASS predictions.
 * These commands are provided solely for compatibility with the GUI, which sends such commands to the KIM device.
 *
 * @author Arribada
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_PREVIPASS_H
#define __MGR_SPI_CMD_PREVIPASS_H

/* Includes ------------------------------------------------------------------*/
#include "mgr_spi_cmd_common.h"

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Read the prepass enable status.
 *
 * This function processes the SPI command to read the current status of the prepass enable configuration,
 * which determines whether satellite PASS predictions are active.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the command.
 * @param[out] tx Pointer to the SPI transmit buffer where the prepass enable status will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_READPREPASSEN_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Initiate a request to write the prepass enable status.
 *
 * This function processes the SPI command that initiates a write request to update the prepass enable configuration.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the write request command.
 * @param[out] tx Pointer to the SPI transmit buffer where the request acknowledgment will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_WRITEPREPASSENREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Write the new prepass enable status.
 *
 * This function processes the SPI command to update the prepass enable configuration after a valid write request.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the new prepass enable value.
 * @param[out] tx Pointer to the SPI transmit buffer where the result of the write operation will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_WRITEPREPASSEN_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Read the current user date and UTC time.
 *
 * This function processes the SPI command to retrieve the current user date and UTC time settings.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the command.
 * @param[out] tx Pointer to the SPI transmit buffer where the date and time information will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_READUDATE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Initiate a request to write the user date and UTC time.
 *
 * This function processes the SPI command that initiates a request to update the current user date and UTC time.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the write request command.
 * @param[out] tx Pointer to the SPI transmit buffer where the request acknowledgment will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_WRITEUDATEREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Write the new user date and UTC time.
 *
 * This function processes the SPI command to update the user date and UTC time after a valid write request.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the new date and time information.
 * @param[out] tx Pointer to the SPI transmit buffer where the result of the update operation will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_WRITEUDATE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

#endif /* __MGR_SPI_CMD_PREVIPASS_H */

/**
 * @}
 */
