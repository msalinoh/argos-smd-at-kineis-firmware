/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_user_data.h
 * @author  Kineis
 * @brief subset of AT commands concerning user data manipulation such as TX
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

/** @brief : convert the input form ASCII to binary.
 *
 * Output data (data after conversion) will be stored in the same buffer.
 *
 * The size of the buffer will be reduced to the half. So 0 will be set onto unused Bytes.
 *
 * Example:
 *           ______________________________________________________________
 * buffer   |  'A' |  'B' |  'C' |  'D' |  'E' |  'F' |  '0' |  '1' |  'F' |
 * before   ---------------------------------------------------------------
 * buffer    ______________________________________________________________
 * after    | 0xAB | 0xCD | 0xEF | 0x01 | 0xF0 | 0x00 | 0x00 | 0x00 | 0x00 |
 *
 * * In this example: u16_charNb is "9 characters" and the return value is "36 bits".
 *
 * @param[in,out] *pu8InputBuffer: pointer to the buffer that contains ASCII data. binary data
 * will be stored into the same buffer.
 * @param[in] u16_charNb: input data buffer length in ASCII char
 *
 * @return data length in bits if everything is OK, 0 otherwise.
 */
uint16_t u16MGR_SPI_CMD_convertAsciiBinary(uint8_t *pu8InputBuffer, uint16_t u16_charNb);

/**
 * @brief
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITETXREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITETXSIZE_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_SPI_CMD_WRITETX_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

#endif /* __MGR_SPI_CMD_USERDATA_H */

/**
 * @}
 */
