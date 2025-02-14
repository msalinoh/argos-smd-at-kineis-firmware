/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_spi_cmd_list_user_data.h
 * @brief Subset of SPI commands concerning user data manipulation such as TX.
 *
 * This header file declares functions used for handling SPI commands related to user data manipulation.
 * These functions include converting ASCII input to binary and processing TX (transmit) commands.
 *
 * @author Arribada
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_USERDATA_H
#define __MGR_SPI_CMD_USERDATA_H

/* Includes ------------------------------------------------------------------*/
#include "kns_types.h"
#include "mgr_spi_cmd_common.h"

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Convert ASCII data to binary.
 *
 * This function converts the input data from its ASCII representation to binary.
 * The conversion is done in-place, and the output (binary) data is stored in the same buffer.
 * The effective size of the buffer is reduced by half, with any unused bytes set to 0.
 *
 * @code
 * // Example:
 * // Before conversion:
 * // buffer = { 'A', 'B', 'C', 'D', 'E', 'F', '0', '1', 'F' }
 * // After conversion:
 * // buffer = { 0xAB, 0xCD, 0xEF, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00 }
 * // In this example, if u16_charNb is 9 characters, the return value is 36 bits.
 * @endcode
 *
 * @param[in,out] pu8InputBuffer Pointer to the buffer containing ASCII data.
 *                                 The binary output is stored in the same buffer.
 * @param[in] u16_charNb Length of the input data in ASCII characters.
 *
 * @return Data length in bits if the conversion is successful, 0 otherwise.
 */
uint16_t u16MGR_SPI_CMD_convertAsciiBinary(uint8_t *pu8InputBuffer, uint16_t u16_charNb);

/**
 * @brief Process the SPI command to initiate a TX request.
 *
 * This function handles the SPI command that initiates a transmission (TX) request.
 * It prepares the necessary settings and acknowledges the reception of the TX request.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the TX request command.
 * @param[out] tx Pointer to the SPI transmit buffer where the response will be sent.
 *
 * @return true if the command is correctly received and processed, false otherwise.
 */
bool bMGR_SPI_CMD_WRITETXREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Process the SPI command to set the TX data size.
 *
 * This function handles the SPI command that specifies the size of the TX data to be transmitted.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the TX size command.
 * @param[out] tx Pointer to the SPI transmit buffer where the acknowledgment or response will be sent.
 *
 * @return true if the command is correctly received and processed, false otherwise.
 */
bool bMGR_SPI_CMD_WRITETXSIZE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Process the SPI command to write TX data.
 *
 * This function handles the SPI command that writes the actual user data to be transmitted.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the TX data.
 * @param[out] tx Pointer to the SPI transmit buffer where the result of the TX write operation will be sent.
 *
 * @return true if the command is correctly received and processed, false otherwise.
 */
bool bMGR_SPI_CMD_WRITETX_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

#endif /* __MGR_SPI_CMD_USERDATA_H */

/**
 * @}
 */
