/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_general.h
 * @author  Kinéis
 * @brief subset of AT commands concerning general purpose (get ID, FW version, ...)
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_AT_CMD_GENERAL_H
#define __MGR_AT_CMD_GENERAL_H

/* Includes ------------------------------------------------------------------*/

#include "mgr_at_cmd_common.h"

/* Functions -----------------------------------------------------------------*/

/** @brief Process AT command "AT+PING" to test the UART communication.
 *
 * "AT+PING=?": if the UART communication is ready the module returns "+OK".
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_PING_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

/** @brief Process AT command "AT+FW" to get the FW version
 *
 * Only status mode is authorized.
 *
 * "AT+FW=?" will log the KIM2 version followed by the ARTIC FW version.
 *
 * Response format: "+FW=<version>"
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_FW_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

/** @brief Process AT command "AT+ID" get the ID of module
 *
 * 1) "AT+ID=<ID>"
 * N/A Response format: "+ERROR=<error_code>". (See \ref ERROR_RETURN_T)
 *
 * 2) "AT+ID=?" returns the ID of module
 * Response format: "+ID=<KIM_ID>" or "+ID=<error_code>". (See \ref ERROR_RETURN_T)
 *
 * "KIM_ID" must be an hexadecimal characters, which can be converted to a 32-bit number.
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_ID_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

#endif /* __MGR_AT_CMD_GENERAL_H */
/**
 * @}
 */
