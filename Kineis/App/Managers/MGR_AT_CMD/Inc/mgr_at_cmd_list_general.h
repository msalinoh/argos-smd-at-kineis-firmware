/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_general.h
 * @author  Kineis
 * @brief subset of AT commands concerning general purpose (get ID, FW version, ...)
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_AT_CMD_LIST_GENERAL_H
#define __MGR_AT_CMD_LIST_GENERAL_H

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
 * \<KIM_ID\> is an 6-digit string, representing the device identifier from the end
 * user point of view.
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_ID_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

/** @brief Process AT command "AT+ADDR" get the address of module
 *
 * 1) "AT+ADDR=<address>"
 * N/A Response format: "+ERROR=<error_code>". (See \ref ERROR_RETURN_T)
 *
 * 2) "AT+ADDR=?" returns the address of module
 * Response format: "+ADDR=<KIM_ADDR>" or "+ADDR=<error_code>". (See \ref ERROR_RETURN_T)
 * \<KIM_ADDR\> is an 8-digit hexadecimal string, corresponding to a 32-bit value. It
 * represents the device identifier from the network point of view.
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_ADDR_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

/** @brief Process AT command "AT+SN" get the serial number of module
 *
 * 1) "AT+SN=<serial_number>"
 * N/A Response format: "+ERROR=<error_code>". (See \ref ERROR_RETURN_T)
 *
 * 2) "AT+SN=?" returns the serial number of module
 * Response format: "+SN=<KIM_SN>" or "+SN=<error_code>". (See \ref ERROR_RETURN_T)
 * \<KIM_SN\> is an 14-digit string, representing the device serial number.
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_SN_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

/** @brief Process AT command "AT+RCONF" get the radio configuration of module
 *
 * 1) "AT+RCONF=<radio configuration>"
 * N/A Response format: "+ERROR=<error_code>". (See \ref ERROR_RETURN_T)
 *
 * Radio configuration format is a 32-ASCII-digit array corresponding to 16 bytes
 *
 * 2) "AT+RCONF=?" returns the radio configuration of module
 * Response format: "+RCONF=<fmin> <fmax> <rflevel> <modulation>" or "+RCONF=<error_code>". (See \ref ERROR_RETURN_T)
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_RCONF_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

#endif /* __MGR_AT_CMD_LIST_GENERAL_H */
/**
 * @}
 */
